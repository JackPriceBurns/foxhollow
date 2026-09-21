#include <aurora/gfx.h>
#include <aurora/imgui.h>
#include <SDL3/SDL.h>
#include <cmath>
#include <cstdint>
#include <cstdlib>

#if defined(FOXHOLLOW_DEBUG_INPUT_TIMING)
extern "C" {
extern uint32_t gPadButtonsJustPressed[];
extern int16_t fearTestMeterMarkerX;
extern uint8_t fearTestMeterInnerHalfWidth;
extern uint8_t fearTestMeterOuterHalfWidth;
extern int16_t gFearTestMeterFadeIn;
extern int16_t gFearTestMeterAlpha;
}
#endif

namespace {
constexpr uint32_t kTextureUploadBudget = 25165824;

bool sVisible = true;
bool sToggleKeyWasDown;
Uint64 sPreviousFrameNs;
Uint64 sSampleStartNs;
float sWorstFrameMs;
float sDisplayedWorstFrameMs;
uint32_t sFrameCount;
float sDisplayedFps;
uint32_t sPreviousCreatedPipelines;
uint32_t sDisplayedPipelinesPerSecond;
uint32_t sPeakTextureUploadSize;
uint32_t sDisplayedPeakTextureUploadSize;
uint32_t sBudgetOverflowFrames;
uint32_t sDisplayedBudgetOverflowFrames;

#if defined(FOXHOLLOW_DEBUG_INPUT_TIMING)
constexpr uint32_t kPadButtonA = 0x0100;
constexpr int kFrameHistory = 128;
constexpr int kPressHistory = 8;
constexpr Uint64 kTimingWindowLingerNs = 10ull * SDL_NS_PER_SECOND;

struct FrameSample {
    uint32_t frame;
    Uint64 beginNs;
    Uint64 presentNs;
    int16_t marker;
    uint8_t inner;
    int16_t alpha;
    int16_t fadeIn;
    bool padA;
};

struct PressSample {
    uint32_t seq;
    const char* source;
    Uint64 keyNs;
    Uint64 pumpNs;
    bool haveSeen;
    uint32_t seenFrame;
    int16_t seenMarker;
    bool havePad;
    uint32_t padFrame;
    Uint64 padFrameBeginNs;
    bool haveStop;
    uint32_t stopFrame;
    int16_t stopMarker;
    uint8_t stopInner;
    bool hit;
    bool havePresent;
    Uint64 stopPresentNs;
};

bool sWatchInstalled;
uint32_t sFrameIndex;
Uint64 sFrameBeginNs;
FrameSample sFrames[kFrameHistory];
uint32_t sFrameWrite;
uint32_t sFramesStored;
PressSample sPresses[kPressHistory];
uint32_t sPressWrite;
uint32_t sPressesStored;
uint32_t sPressSeq;
bool sSpaceDown;
bool sPadADown;
Uint64 sLastPressNs;
int16_t sPreviousFadeIn;
int16_t sPreviousMarker;
bool sHaveMarkerSample;

FrameSample* frameAt(uint32_t back) {
    if (back >= sFramesStored) {
        return nullptr;
    }
    return &sFrames[(sFrameWrite + kFrameHistory - 1 - back) % kFrameHistory];
}

PressSample* pressAt(uint32_t back) {
    if (back >= sPressesStored) {
        return nullptr;
    }
    return &sPresses[(sPressWrite + kPressHistory - 1 - back) % kPressHistory];
}

void recordPress(const char* source, Uint64 keyNs) {
    PressSample& press = sPresses[sPressWrite];
    sPressWrite = (sPressWrite + 1) % kPressHistory;
    if (sPressesStored < kPressHistory) {
        sPressesStored++;
    }
    press = PressSample{};
    press.seq = ++sPressSeq;
    press.source = source;
    press.keyNs = keyNs;
    press.pumpNs = SDL_GetTicksNS();
    sLastPressNs = press.pumpNs;

    for (uint32_t back = 0; back < sFramesStored; back++) {
        const FrameSample* frame = frameAt(back);
        if (frame->presentNs != 0 && frame->presentNs <= keyNs) {
            press.haveSeen = true;
            press.seenFrame = frame->frame;
            press.seenMarker = frame->marker;
            break;
        }
    }
    if (!press.haveSeen) {
        for (uint32_t back = 0; back < sFramesStored; back++) {
            const FrameSample* frame = frameAt(back);
            if (frame->presentNs != 0) {
                press.haveSeen = true;
                press.seenFrame = frame->frame;
                press.seenMarker = frame->marker;
                break;
            }
        }
    }
}

bool SDLCALL eventWatch(void*, SDL_Event* event) {
    switch (event->type) {
    case SDL_EVENT_KEY_DOWN:
        if (event->key.scancode == SDL_SCANCODE_SPACE) {
            if (!event->key.repeat) {
                recordPress("SPACE", event->key.timestamp);
            }
            sSpaceDown = true;
        }
        break;
    case SDL_EVENT_KEY_UP:
        if (event->key.scancode == SDL_SCANCODE_SPACE) {
            sSpaceDown = false;
        }
        break;
    case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
        if (event->gbutton.button == SDL_GAMEPAD_BUTTON_SOUTH) {
            recordPress("PAD A", event->gbutton.timestamp);
            sPadADown = true;
        }
        break;
    case SDL_EVENT_GAMEPAD_BUTTON_UP:
        if (event->gbutton.button == SDL_GAMEPAD_BUTTON_SOUTH) {
            sPadADown = false;
        }
        break;
    default:
        break;
    }
    return true;
}

float msSince(Uint64 from, Uint64 to) {
    return to >= from ? static_cast<float>(to - from) / 1000000.0f : -static_cast<float>(from - to) / 1000000.0f;
}

void sampleTimingFrame(Uint64 now) {
    const bool padA = (gPadButtonsJustPressed[0] & kPadButtonA) != 0;
    const int16_t marker = fearTestMeterMarkerX;
    const uint8_t inner = fearTestMeterInnerHalfWidth;
    const int16_t alpha = gFearTestMeterAlpha;
    const int16_t fadeIn = gFearTestMeterFadeIn;

    FrameSample& frame = sFrames[sFrameWrite];
    sFrameWrite = (sFrameWrite + 1) % kFrameHistory;
    if (sFramesStored < kFrameHistory) {
        sFramesStored++;
    }
    frame = FrameSample{};
    frame.frame = sFrameIndex;
    frame.beginNs = sFrameBeginNs;
    frame.marker = marker;
    frame.inner = inner;
    frame.alpha = alpha;
    frame.fadeIn = fadeIn;
    frame.padA = padA;

    PressSample* press = pressAt(0);
    if (padA && press != nullptr && !press->havePad) {
        press->havePad = true;
        press->padFrame = sFrameIndex;
        press->padFrameBeginNs = sFrameBeginNs;
    }
    if (sHaveMarkerSample && sPreviousFadeIn != 0 && fadeIn == 0 && alpha > 0 && press != nullptr && !press->haveStop) {
        press->haveStop = true;
        press->stopFrame = sFrameIndex;
        press->stopMarker = marker;
        press->stopInner = inner;
        press->hit = std::abs(static_cast<int>(marker)) <= static_cast<int>(inner);
    }
    sPreviousFadeIn = fadeIn;
    sPreviousMarker = marker;
    sHaveMarkerSample = true;
    (void)now;
}

void drawMeterMock(float scale) {
    ImDrawList* draw = ImGui::GetWindowDrawList();
    const ImVec2 origin = ImGui::GetCursorScreenPos();
    const float outer = static_cast<float>(fearTestMeterOuterHalfWidth != 0 ? fearTestMeterOuterHalfWidth : 96);
    const float inner = static_cast<float>(fearTestMeterInnerHalfWidth);
    const float width = outer * 2.0f * scale;
    const float height = 22.0f;
    const float centerX = origin.x + width * 0.5f;
    const float top = origin.y + 4.0f;
    const float bottom = top + height;

    draw->AddRectFilled(ImVec2(origin.x, top), ImVec2(origin.x + width, bottom), IM_COL32(40, 40, 40, 230));
    draw->AddRectFilled(ImVec2(centerX - inner * scale, top), ImVec2(centerX + inner * scale, bottom),
                        IM_COL32(60, 170, 70, 230));
    draw->AddRect(ImVec2(origin.x, top), ImVec2(origin.x + width, bottom), IM_COL32(200, 200, 200, 255));

    const PressSample* press = pressAt(0);
    if (press != nullptr && press->haveSeen) {
        const float x = centerX + static_cast<float>(press->seenMarker) * scale;
        draw->AddLine(ImVec2(x, top - 4.0f), ImVec2(x, bottom + 4.0f), IM_COL32(255, 255, 255, 255), 2.0f);
    }
    if (press != nullptr && press->haveStop) {
        const float x = centerX + static_cast<float>(press->stopMarker) * scale;
        draw->AddLine(ImVec2(x, top - 4.0f), ImVec2(x, bottom + 4.0f), IM_COL32(255, 220, 40, 255), 2.0f);
    }
    const float liveX = centerX + static_cast<float>(fearTestMeterMarkerX) * scale;
    draw->AddRectFilled(ImVec2(liveX - 2.0f * scale, top), ImVec2(liveX + 2.0f * scale, bottom),
                        IM_COL32(255, 40, 40, 255));

    ImGui::Dummy(ImVec2(width, height + 10.0f));
}

void drawTimingWindow(Uint64 now) {
    const bool meterVisible = gFearTestMeterAlpha > 0;
    const bool recentPress = sLastPressNs != 0 && now - sLastPressNs < kTimingWindowLingerNs;
    if (!meterVisible && !recentPress) {
        return;
    }

    const ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
                                   ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing |
                                   ImGuiWindowFlags_NoInputs;
    ImGui::SetNextWindowPos(ImVec2(10.0f, 110.0f), ImGuiCond_Always);
    ImGui::SetNextWindowBgAlpha(0.75f);
    ImGui::Begin("LightFoot Timing", nullptr, flags);
    ImGui::SetWindowFontScale(1.35f);

    const ImVec4 hot = ImVec4(1.0f, 0.45f, 0.35f, 1.0f);
    const ImVec4 good = ImVec4(0.45f, 1.0f, 0.5f, 1.0f);
    const ImVec4 dim = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);

    const FrameSample* current = frameAt(0);
    const FrameSample* previous = frameAt(1);
    const float speed =
        (current != nullptr && previous != nullptr) ? static_cast<float>(current->marker - previous->marker) : 0.0f;

    ImGui::Text("frame %u   marker %+d px   zone +/-%d   speed %+.1f px/frame", sFrameIndex, fearTestMeterMarkerX,
                fearTestMeterInnerHalfWidth, speed);
    ImGui::Text("meter %s  alpha %d", gFearTestMeterFadeIn != 0 ? "RUNNING" : "stopped", gFearTestMeterAlpha);

    if (sSpaceDown || sPadADown) {
        ImGui::TextColored(hot, "[ A DOWN ]");
    } else {
        ImGui::TextColored(dim, "[ A up ]");
    }

    drawMeterMock(2.0f);
    ImGui::TextColored(dim, "red = live marker   white = marker on screen at keypress   yellow = where it stopped");

    const PressSample* press = pressAt(0);
    if (press == nullptr) {
        ImGui::TextColored(dim, "press A / SPACE while the meter runs");
        ImGui::End();
        return;
    }

    ImGui::Separator();
    ImGui::Text("press #%u  (%s)", press->seq, press->source);
    if (press->haveSeen) {
        ImGui::Text("  key event        0.0 ms   on-screen marker %+d (frame %u)", press->seenMarker, press->seenFrame);
    } else {
        ImGui::Text("  key event        0.0 ms");
    }
    ImGui::Text("  SDL delivered  %+6.1f ms", msSince(press->keyNs, press->pumpNs));
    if (press->havePad) {
        ImGui::Text("  pad saw A      %+6.1f ms   frame %u (+%d frames)", msSince(press->keyNs, press->padFrameBeginNs),
                    press->padFrame, press->haveSeen ? static_cast<int>(press->padFrame - press->seenFrame) : 0);
    } else {
        ImGui::TextColored(dim, "  pad saw A      ...");
    }
    if (press->haveStop) {
        const int delta = static_cast<int>(press->stopMarker) - static_cast<int>(press->seenMarker);
        ImGui::Text("  meter stopped    frame %u   marker %+d   zone +/-%d", press->stopFrame, press->stopMarker,
                    press->stopInner);
        ImGui::TextColored(press->hit ? good : hot, "  %s", press->hit ? "HIT" : "MISS");
        if (press->haveSeen) {
            ImGui::Text("  shown -> stopped  %+d -> %+d   delta %d px   +%d frames", press->seenMarker,
                        press->stopMarker, delta, static_cast<int>(press->stopFrame - press->seenFrame));
        }
        if (press->havePresent) {
            ImGui::Text("  stop frame presented %+6.1f ms", msSince(press->keyNs, press->stopPresentNs));
        }
    } else {
        ImGui::TextColored(dim, "  meter stopped    ...");
    }

    if (sPressesStored > 1) {
        ImGui::Separator();
        ImGui::TextColored(dim, "history (newest first): #seq shown->stopped delta frames result");
        for (uint32_t back = 1; back < sPressesStored; back++) {
            const PressSample* old = pressAt(back);
            if (old->haveStop && old->haveSeen) {
                ImGui::Text("  #%u  %+d -> %+d   %d px   +%d fr   %s", old->seq, old->seenMarker, old->stopMarker,
                            static_cast<int>(old->stopMarker) - static_cast<int>(old->seenMarker),
                            static_cast<int>(old->stopFrame - old->seenFrame), old->hit ? "HIT" : "MISS");
            } else {
                ImGui::Text("  #%u  (no meter stop seen)", old->seq);
            }
        }
    }

    ImGui::End();
}
#endif
} // namespace

#if defined(FOXHOLLOW_DEBUG_INPUT_TIMING)
extern "C" void fhDebugOverlayFrameBegin(void) {
    if (!sWatchInstalled) {
        sWatchInstalled = true;
        SDL_AddEventWatch(eventWatch, nullptr);
    }
    sFrameIndex++;
    sFrameBeginNs = SDL_GetTicksNS();
}

extern "C" void fhDebugOverlayFramePresented(void) {
    const Uint64 now = SDL_GetTicksNS();
    FrameSample* frame = frameAt(0);
    if (frame != nullptr && frame->presentNs == 0) {
        frame->presentNs = now;
        PressSample* press = pressAt(0);
        if (press != nullptr && press->haveStop && !press->havePresent && press->stopFrame == frame->frame) {
            press->havePresent = true;
            press->stopPresentNs = now;
        }
    }
}
#endif

extern "C" void fhDebugOverlayDraw(void) {
    const Uint64 now = SDL_GetTicksNS();
    const float frameMs = sPreviousFrameNs != 0 ? static_cast<float>(now - sPreviousFrameNs) / 1000000.0f : 0.0f;
    sPreviousFrameNs = now;

#if defined(FOXHOLLOW_DEBUG_INPUT_TIMING)
    sampleTimingFrame(now);
#endif

    const AuroraStats* stats = aurora_get_stats();

    if (sSampleStartNs == 0) {
        sSampleStartNs = now;
        if (stats != nullptr) {
            sPreviousCreatedPipelines = stats->createdPipelines;
        }
    }
    if (frameMs > sWorstFrameMs) {
        sWorstFrameMs = frameMs;
    }
    sFrameCount++;
    if (stats != nullptr) {
        if (stats->lastTextureUploadSize > sPeakTextureUploadSize) {
            sPeakTextureUploadSize = stats->lastTextureUploadSize;
        }
        if (stats->lastTextureUploadSize >= kTextureUploadBudget) {
            sBudgetOverflowFrames++;
        }
    }

    if (now - sSampleStartNs >= SDL_NS_PER_SECOND) {
        const float elapsedSeconds = static_cast<float>(now - sSampleStartNs) / 1000000000.0f;
        sDisplayedWorstFrameMs = sWorstFrameMs;
        sDisplayedFps = elapsedSeconds > 0.0f ? static_cast<float>(sFrameCount) / elapsedSeconds : 0.0f;
        sDisplayedPeakTextureUploadSize = sPeakTextureUploadSize;
        sDisplayedBudgetOverflowFrames = sBudgetOverflowFrames;
        if (stats != nullptr) {
            sDisplayedPipelinesPerSecond = stats->createdPipelines - sPreviousCreatedPipelines;
            sPreviousCreatedPipelines = stats->createdPipelines;
        }
        sWorstFrameMs = 0.0f;
        sFrameCount = 0;
        sPeakTextureUploadSize = 0;
        sBudgetOverflowFrames = 0;
        sSampleStartNs = now;
    }

    int keyCount;
    const bool* keys = SDL_GetKeyboardState(&keyCount);
    const bool toggleKeyDown = keyCount > SDL_SCANCODE_COMMA && keys[SDL_SCANCODE_COMMA];
    if (toggleKeyDown && !sToggleKeyWasDown) {
        sVisible = !sVisible;
    }
    sToggleKeyWasDown = toggleKeyDown;

    if (!sVisible) {
        return;
    }

    const float displayedWorstFrameMs = sDisplayedWorstFrameMs != 0.0f ? sDisplayedWorstFrameMs : sWorstFrameMs;
    const ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
                                   ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing |
                                   ImGuiWindowFlags_NoInputs;
    ImGui::SetNextWindowPos(ImVec2(10.0f, 10.0f), ImGuiCond_Always);
    ImGui::SetNextWindowBgAlpha(0.65f);
    ImGui::Begin("Foxhollow Performance", nullptr, flags);
    ImGui::Text("%.1f FPS  %.2f ms  worst %.2f ms", sDisplayedFps, frameMs, displayedWorstFrameMs);

    if (stats != nullptr) {
        const ImVec4 hot = ImVec4(1.0f, 0.45f, 0.35f, 1.0f);
        const ImVec4 warm = ImVec4(1.0f, 0.85f, 0.35f, 1.0f);

        if (stats->queuedPipelines > 0 || sDisplayedPipelinesPerSecond > 0) {
            ImGui::TextColored(warm, "pipelines  queued %u  built/s %u", stats->queuedPipelines,
                               sDisplayedPipelinesPerSecond);
        } else {
            ImGui::Text("pipelines  queued 0  built/s 0");
        }

        const float peakMiB = static_cast<float>(sDisplayedPeakTextureUploadSize) / 1048576.0f;
        const float budgetMiB = static_cast<float>(kTextureUploadBudget) / 1048576.0f;
        if (sDisplayedBudgetOverflowFrames > 0) {
            ImGui::TextColored(hot, "texupload  peak %.2f / %.1f MiB  OVERFLOW %u frames", peakMiB, budgetMiB,
                               sDisplayedBudgetOverflowFrames);
        } else {
            ImGui::Text("texupload  peak %.2f / %.1f MiB", peakMiB, budgetMiB);
        }

        ImGui::Text("draws  %u  merged %u", stats->drawCallCount, stats->mergedDrawCallCount);
    }

    ImGui::End();

#if defined(FOXHOLLOW_DEBUG_INPUT_TIMING)
    drawTimingWindow(now);
#endif
}
