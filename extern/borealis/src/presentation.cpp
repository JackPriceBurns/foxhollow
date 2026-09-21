#include <borealis/presentation.hpp>

#include <cmath>

#if defined(__ANDROID__)
#include <SDL3/SDL_system.h>
#include <jni.h>
#endif

namespace borealis::presentation {

#if defined(__ANDROID__)
namespace {

bool clear_pending_exception(JNIEnv* env) noexcept {
    if (env == nullptr || !env->ExceptionCheck()) {
        return false;
    }
    env->ExceptionClear();
    return true;
}

}  // namespace
#endif

bool set_preferred_frame_rate(float framesPerSecond) noexcept {
    if (!std::isfinite(framesPerSecond) || framesPerSecond < 0.0f) {
        return false;
    }

#if defined(__ANDROID__)
    auto* env = static_cast<JNIEnv*>(SDL_GetAndroidJNIEnv());
    if (env == nullptr) {
        return false;
    }

    jobject activity = static_cast<jobject>(SDL_GetAndroidActivity());
    if (activity == nullptr || clear_pending_exception(env)) {
        if (activity != nullptr) {
            env->DeleteLocalRef(activity);
        }
        return false;
    }

    jclass activityClass = env->GetObjectClass(activity);
    if (activityClass == nullptr || clear_pending_exception(env)) {
        env->DeleteLocalRef(activity);
        return false;
    }

    jmethodID method =
        env->GetMethodID(activityClass, "setPreferredSurfaceFrameRate", "(F)V");
    env->DeleteLocalRef(activityClass);
    if (method == nullptr || clear_pending_exception(env)) {
        env->DeleteLocalRef(activity);
        return false;
    }

    jvalue args[1]{};
    args[0].f = framesPerSecond;
    env->CallVoidMethodA(activity, method, args);
    const bool succeeded = !clear_pending_exception(env);
    env->DeleteLocalRef(activity);
    return succeeded;
#else
    (void)framesPerSecond;
    return false;
#endif
}

}  // namespace borealis::presentation
