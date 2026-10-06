#include "websocket.hpp"

#import <Foundation/Foundation.h>

@interface FhWebSocketDelegate : NSObject <NSURLSessionWebSocketDelegate>
- (instancetype)initWithQueue:(std::shared_ptr<fh::net::EventQueue>)queue;
@end

@implementation FhWebSocketDelegate {
  std::shared_ptr<fh::net::EventQueue> _queue;
}

- (instancetype)initWithQueue:(std::shared_ptr<fh::net::EventQueue>)queue {
  self = [super init];
  if (self != nil) {
    _queue = std::move(queue);
  }
  return self;
}

- (void)URLSession:(NSURLSession*)session
          webSocketTask:(NSURLSessionWebSocketTask*)task
    didOpenWithProtocol:(NSString*)protocol {
  _queue->push({fh::net::SocketEvent::Type::Open, {}, 0});
}

- (void)URLSession:(NSURLSession*)session
       webSocketTask:(NSURLSessionWebSocketTask*)task
    didCloseWithCode:(NSURLSessionWebSocketCloseCode)code
              reason:(NSData*)reason {
  _queue->push({fh::net::SocketEvent::Type::Closed, {}, static_cast<int>(code)});
}

- (void)URLSession:(NSURLSession*)session task:(NSURLSessionTask*)task didCompleteWithError:(NSError*)error {
  int code = 1006;
  if ([task.response isKindOfClass:[NSHTTPURLResponse class]]) {
    const NSInteger status = static_cast<NSHTTPURLResponse*>(task.response).statusCode;
    if (status != 101) {
      code = static_cast<int>(status);
    }
  }
  _queue->push({fh::net::SocketEvent::Type::Closed, {}, code});
}
@end

namespace fh::net {
namespace {

void receive(NSURLSessionWebSocketTask* task, std::shared_ptr<EventQueue> queue) {
  [task receiveMessageWithCompletionHandler:^(NSURLSessionWebSocketMessage* message, NSError* error) {
    if (error != nil || message == nil) {
      return;
    }
    if (message.type == NSURLSessionWebSocketMessageTypeString) {
      queue->push({SocketEvent::Type::Text, std::string(message.string.UTF8String), 0});
    } else {
      queue->push({SocketEvent::Type::Binary,
                   std::string(static_cast<const char*>(message.data.bytes), message.data.length), 0});
    }
    receive(task, queue);
  }];
}

class UrlSessionWebSocket final : public WebSocket {
public:
  UrlSessionWebSocket(const std::string& url, const std::string& bearer) : mQueue(std::make_shared<EventQueue>()) {
    mDelegate = [[FhWebSocketDelegate alloc] initWithQueue:mQueue];
    NSURL* nsUrl = [NSURL URLWithString:[NSString stringWithUTF8String:url.c_str()]];
    if (nsUrl == nil) {
      mQueue->push({SocketEvent::Type::Closed, {}, 1015});
      return;
    }
    NSURLSessionConfiguration* configuration = [NSURLSessionConfiguration ephemeralSessionConfiguration];
    mSession = [NSURLSession sessionWithConfiguration:configuration delegate:mDelegate delegateQueue:nil];
    NSMutableURLRequest* request = [NSMutableURLRequest requestWithURL:nsUrl];
    if (!bearer.empty()) {
      [request setValue:[@"Bearer " stringByAppendingString:[NSString stringWithUTF8String:bearer.c_str()]]
          forHTTPHeaderField:@"Authorization"];
    }
    mTask = [mSession webSocketTaskWithRequest:request];
    mTask.maximumMessageSize = 1 << 20;
    [mTask resume];
    receive(mTask, mQueue);
  }

  ~UrlSessionWebSocket() override {
    [mTask cancelWithCloseCode:NSURLSessionWebSocketCloseCodeNormalClosure reason:nil];
    [mSession invalidateAndCancel];
  }

  void send_binary(const void* data, size_t size) override {
    if (mTask == nil) {
      return;
    }
    NSData* payload = [NSData dataWithBytes:data length:size];
    [mTask sendMessage:[[NSURLSessionWebSocketMessage alloc] initWithData:payload]
        completionHandler:^(NSError* error){
        }];
  }

  void send_text(std::string_view text) override {
    if (mTask == nil) {
      return;
    }
    NSString* payload = [[NSString alloc] initWithBytes:text.data() length:text.size() encoding:NSUTF8StringEncoding];
    [mTask sendMessage:[[NSURLSessionWebSocketMessage alloc] initWithString:payload]
        completionHandler:^(NSError* error){
        }];
  }

  bool poll(SocketEvent& out) override { return mQueue->pop(out); }

private:
  std::shared_ptr<EventQueue> mQueue;
  FhWebSocketDelegate* mDelegate = nil;
  NSURLSession* mSession = nil;
  NSURLSessionWebSocketTask* mTask = nil;
};

} // namespace

std::unique_ptr<WebSocket> connect_websocket(const std::string& url, const std::string& bearer) {
  return std::make_unique<UrlSessionWebSocket>(url, bearer);
}

} // namespace fh::net
