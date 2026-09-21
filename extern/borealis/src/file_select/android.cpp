#include "file_select_internal.hpp"

#include <SDL3/SDL_system.h>

#include <jni.h>

#include <memory>
#include <string>
#include <utility>

namespace borealis::file_select::detail {
namespace {

bool clear_pending_exception(JNIEnv* env) {
    if (env == nullptr || !env->ExceptionCheck()) {
        return false;
    }
    env->ExceptionClear();
    return true;
}

std::string to_string(JNIEnv* env, jstring value) {
    if (env == nullptr || value == nullptr) {
        return {};
    }

    const char* utf8 = env->GetStringUTFChars(value, nullptr);
    if (utf8 == nullptr) {
        clear_pending_exception(env);
        return {};
    }

    std::string result{utf8};
    env->ReleaseStringUTFChars(value, utf8);
    return result;
}

struct ActivityMethod {
    JNIEnv* env = nullptr;
    jobject activity = nullptr;
    jmethodID method = nullptr;

    ActivityMethod() = default;
    ActivityMethod(const ActivityMethod&) = delete;
    ActivityMethod& operator=(const ActivityMethod&) = delete;
    ActivityMethod(ActivityMethod&& other) noexcept
        : env{std::exchange(other.env, nullptr)}, activity{std::exchange(other.activity, nullptr)},
          method{std::exchange(other.method, nullptr)} {}
    ActivityMethod& operator=(ActivityMethod&&) = delete;

    ~ActivityMethod() {
        if (env != nullptr && activity != nullptr) {
            env->DeleteLocalRef(activity);
        }
    }
};

ActivityMethod get_activity_method(const char* name, const char* signature) {
    ActivityMethod result;
    result.env = static_cast<JNIEnv*>(SDL_GetAndroidJNIEnv());
    if (result.env == nullptr) {
        return result;
    }

    result.activity = static_cast<jobject>(SDL_GetAndroidActivity());
    if (result.activity == nullptr || clear_pending_exception(result.env)) {
        return result;
    }

    jclass activityClass = result.env->GetObjectClass(result.activity);
    if (activityClass == nullptr || clear_pending_exception(result.env)) {
        return result;
    }
    result.method = result.env->GetMethodID(activityClass, name, signature);
    result.env->DeleteLocalRef(activityClass);
    if (clear_pending_exception(result.env)) {
        result.method = nullptr;
    }
    return result;
}

struct AndroidFolderState {
    Callback callback;
};

bool show_folder_dialog(AndroidFolderState* state) {
    auto method = get_activity_method("showFolderDialog", "(J)Z");
    if (method.method == nullptr) {
        return false;
    }

    const jboolean shown = method.env->CallBooleanMethod(
        method.activity, method.method, reinterpret_cast<jlong>(state));
    if (clear_pending_exception(method.env)) {
        return false;
    }
    return shown == JNI_TRUE;
}

}  // namespace

std::string android_display_name(std::string_view location) {
    auto method =
        get_activity_method("getDisplayNameForUri", "(Ljava/lang/String;)Ljava/lang/String;");
    if (method.method == nullptr) {
        return {};
    }

    const std::string locationString{location};
    jstring uri = method.env->NewStringUTF(locationString.c_str());
    if (uri == nullptr || clear_pending_exception(method.env)) {
        return {};
    }

    auto displayName =
        static_cast<jstring>(method.env->CallObjectMethod(method.activity, method.method, uri));
    method.env->DeleteLocalRef(uri);
    if (displayName == nullptr || clear_pending_exception(method.env)) {
        return {};
    }

    std::string result = to_string(method.env, displayName);
    method.env->DeleteLocalRef(displayName);
    return result;
}

void open_android_folder(FolderOptions, Callback callback) {
    auto state =
        std::make_unique<AndroidFolderState>(AndroidFolderState{.callback = std::move(callback)});
    if (show_folder_dialog(state.get())) {
        state.release();
        return;
    }

    complete(std::move(state->callback), {
                                             .status = Status::Failed,
                                             .message = "Unable to open the Android folder dialog",
                                         });
}

}  // namespace borealis::file_select::detail

extern "C" JNIEXPORT void JNICALL
Java_dev_encounter_borealis_BorealisActivity_nativeFolderDialogResult(
    JNIEnv* env, jclass, jlong userdata, jstring path, jstring error) {
    using namespace borealis::file_select;
    using namespace borealis::file_select::detail;

    std::unique_ptr<AndroidFolderState> state{reinterpret_cast<AndroidFolderState*>(userdata)};
    if (state == nullptr) {
        return;
    }

    const std::string pathString = to_string(env, path);
    const std::string errorString = to_string(env, error);
    if (!errorString.empty()) {
        complete(std::move(state->callback), {
                                                 .status = Status::Failed,
                                                 .message = errorString,
                                             });
    } else if (pathString.empty()) {
        complete(std::move(state->callback), {.status = Status::Canceled});
    } else {
        complete(std::move(state->callback), {
                                                 .status = Status::Selected,
                                                 .locations = {pathString},
                                             });
    }
}
