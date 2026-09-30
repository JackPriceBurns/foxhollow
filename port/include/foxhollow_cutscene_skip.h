#ifndef FOXHOLLOW_CUTSCENE_SKIP_H_
#define FOXHOLLOW_CUTSCENE_SKIP_H_

#ifdef __cplusplus
extern "C" {
#endif

extern unsigned char gFhCutsceneSkipWorldEffect;
extern unsigned char gFhCutsceneSkipMuteSfx;
extern unsigned char gFhCutsceneSkipHideSubtitles;
extern unsigned char gFhCutsceneSkipEndSubtitles;

void fhCutsceneSkipKeyDown(int scancode);
void fhCutsceneSkipUpdate(void);
void fhCutsceneSkipRunFrame(void);
int fhCutsceneSkipIsActive(void);
void fhCutsceneSkipNotify(const char* text, int holdWhileSkipping);
void fhCutsceneSkipDrawNotification(void);

#ifdef __cplusplus
}
#endif

#endif
