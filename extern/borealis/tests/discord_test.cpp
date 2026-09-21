#include "borealis/discord.hpp"

#include <gtest/gtest.h>

#include "discord/discord_protocol.hpp"

#include <nlohmann/json.hpp>

#include <algorithm>
#include <string>
#include <vector>

namespace {

namespace detail = borealis::discord::detail;
using json = nlohmann::json;

class FakeSocket {
public:
    bool write(const detail::Frame& frame) {
        const auto encoded = detail::encode_frame(frame);
        if (encoded.empty()) {
            return false;
        }
        wire.insert(wire.end(), encoded.begin(), encoded.end());
        return true;
    }

    void receive(size_t count, std::vector<uint8_t>& pending) {
        count = std::min(count, wire.size());
        pending.insert(pending.end(), wire.begin(), wire.begin() + count);
        wire.erase(wire.begin(), wire.begin() + count);
    }

    std::vector<uint8_t> wire;
};

TEST(DiscordProtocol, HandshakeFramingThroughFakeSocket) {
    FakeSocket socket;
    ASSERT_TRUE(socket.write(detail::make_handshake_frame("123456")));
    ASSERT_GT(socket.wire.size(), detail::FrameHeaderSize);
    EXPECT_EQ(socket.wire[0], 0);
    EXPECT_EQ(socket.wire[1], 0);
    EXPECT_EQ(socket.wire[2], 0);
    EXPECT_EQ(socket.wire[3], 0);

    std::vector<uint8_t> pending;
    detail::Frame frame;
    socket.receive(5, pending);
    EXPECT_EQ(detail::decode_frame(pending, frame), detail::DecodeStatus::None);
    socket.receive(socket.wire.size(), pending);
    ASSERT_EQ(detail::decode_frame(pending, frame), detail::DecodeStatus::Frame);
    EXPECT_EQ(frame.opcode, detail::Opcode::Handshake);
    const json payload = json::parse(frame.payload);
    EXPECT_EQ(payload.at("v"), 1);
    EXPECT_EQ(payload.at("client_id"), "123456");
    EXPECT_TRUE(pending.empty());
}

TEST(DiscordProtocol, ActivityPayload) {
    borealis::discord::Presence presence{
        .state = "3/10 hearts",
        .details = "Ordon Village",
        .startTimestamp = 42,
        .largeImageKey = "icon",
        .largeImageText = "Dusklight",
    };
    const detail::Frame frame = detail::make_set_activity_frame("nonce", 1234, presence);
    EXPECT_EQ(frame.opcode, detail::Opcode::Frame);
    const json payload = json::parse(frame.payload);
    EXPECT_EQ(payload.at("cmd"), "SET_ACTIVITY");
    EXPECT_EQ(payload.at("nonce"), "nonce");
    EXPECT_EQ(payload.at("args").at("pid"), 1234);
    EXPECT_EQ(payload.at("args").at("activity").at("state"), "3/10 hearts");
    EXPECT_EQ(payload.at("args").at("activity").at("details"), "Ordon Village");
    EXPECT_EQ(payload.at("args").at("activity").at("timestamps").at("start"), 42);
    EXPECT_EQ(payload.at("args").at("activity").at("assets").at("large_image"), "icon");

    const json cleared =
        json::parse(detail::make_set_activity_frame("clear", 1234, std::nullopt).payload);
    EXPECT_TRUE(cleared.at("args").at("activity").is_null());
}

TEST(DiscordProtocol, RejectsOversizedFrame) {
    detail::Frame oversized{
        .payload = std::string(detail::MaxFramePayloadSize + 1, 'x'),
    };
    EXPECT_TRUE(detail::encode_frame(oversized).empty());

    std::vector<uint8_t> corrupt(detail::FrameHeaderSize);
    const uint32_t length = static_cast<uint32_t>(detail::MaxFramePayloadSize + 1);
    corrupt[4] = static_cast<uint8_t>(length & 0xff);
    corrupt[5] = static_cast<uint8_t>((length >> 8) & 0xff);
    corrupt[6] = static_cast<uint8_t>((length >> 16) & 0xff);
    corrupt[7] = static_cast<uint8_t>((length >> 24) & 0xff);
    detail::Frame frame;
    EXPECT_EQ(detail::decode_frame(corrupt, frame), detail::DecodeStatus::Corrupt);
}

TEST(DiscordProtocol, DirtyPresenceState) {
    detail::PresenceState state;
    const borealis::discord::Presence first{
        .state = "Playing",
        .details = "Title screen",
    };
    EXPECT_TRUE(state.update(first));
    EXPECT_FALSE(state.update(first));
    EXPECT_EQ(state.take_dirty(), first);
    EXPECT_FALSE(state.take_dirty());

    state.mark_dirty();
    EXPECT_EQ(state.take_dirty(), first);
    EXPECT_TRUE(state.clear());
    EXPECT_FALSE(state.clear());
    EXPECT_TRUE(state.take_clear_dirty());
    EXPECT_FALSE(state.take_clear_dirty());
}

}  // namespace
