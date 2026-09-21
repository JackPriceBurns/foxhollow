# Borealis Android kit

Shared Android platform layer for Aurora ports:

- SDL activity with immersive mode, runtime arguments, surface lifecycle
  synchronization, and preferred frame-rate handling.
- DocumentsProvider for the app's internal files directory.
- Gradle staging for the native library, libc++, assets, Java sources, ABI
  splits, and shared ProGuard rules.

Ports provide their own package ID, manifest, resources, and signing.

## Native integration

Create the Android executable as a shared library, link Aurora and Borealis, then register the target:

```cmake
if (ANDROID)
    add_library(my_port SHARED ${MY_PORT_SOURCES})
else ()
    add_executable(my_port ${MY_PORT_SOURCES})
endif ()
target_link_libraries(my_port PRIVATE aurora::aurora borealis::core)
borealis_configure_android_application(my_port)
```

On Android, this:

- names the output `libmain.so`;
- retains `SDL_main` and enables a SHA-1 build ID; and
- generates `borealis-android.properties` in the CMake build directory.

Configure and build CMake before invoking Gradle.

### Surface frame rate

Link `borealis::presentation` and set the frame rate after `aurora_initialize`:

```cpp
#include <borealis/presentation.hpp>

borealis::presentation::set_preferred_frame_rate(120.0f);
```

Positive values request a specific rate, zero uses the display's highest rate. The
value persists across surface recreation. Call again after a runtime setting change.

## Gradle integration

Define `ext.borealisAndroid` in the application module and apply the shared
script:

```groovy
def repoDir = rootProject.projectDir.parentFile.parentFile
def borealisDir = new File(repoDir, 'extern/borealis')

ext.borealisAndroid = [
    borealisDir: borealisDir,
    propertiesFile: new File(repoDir, 'build/android-arm64/borealis-android.properties'),
    namespace: 'dev.example.myport',
    applicationId: 'dev.example.myport',
    abis: ['arm64-v8a'],
    assets: [
        [from: new File(repoDir, 'res'), into: 'res']
    ],
    proguardRules: file('proguard-rules.pro')
]

apply from: new File(borealisDir, 'platforms/android/gradle/borealis-application.gradle')
```

Required configuration keys are `borealisDir`, `propertiesFile`,
`namespace`, and `applicationId`. Optional keys are `abis`, `assets`,
`compileSdk`, `minSdk`, `targetSdk`, `ndkVersion`, and
`proguardRules`. Each asset entry accepts `from`, `into`, `includes`,
and `excludes`.

The script requires `ANDROID_HOME` or `ANDROID_SDK_ROOT`. Set the NDK version with
`ndkVersion` or `ANDROID_NDK_VERSION`. Gradle's `minSdk` defaults to CMake's
`ANDROID_PLATFORM`.

## Activity and manifest

A port activity can inherit the shared behavior:

```java
public final class MyPortActivity extends BorealisActivity {
}
```

Register the activity normally. To expose internal files in Android's Files UI,
also register the shared provider:

```xml
<provider
    android:name="dev.encounter.borealis.BorealisDocumentsProvider"
    android:authorities="${applicationId}.documents"
    android:exported="true"
    android:grantUriPermissions="true"
    android:permission="android.permission.MANAGE_DOCUMENTS">
    <intent-filter>
        <action android:name="android.content.action.DOCUMENTS_PROVIDER" />
    </intent-filter>
</provider>
```

The provider supports browsing, creation, rename, deletion, and read/write access.
Its root is hidden when `data_location.json` selects a non-default data location.

The shared activity accepts either a `borealis_argv` string array or a
shell-like `borealis_args` string in its launch intent.

## AVD rendering

Aurora requires hardware graphics. Start AVDs with host GPU acceleration:

```bash
emulator -avd <name> -gpu host
```
