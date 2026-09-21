#ifndef FOXHOLLOW_MODS_H_
#define FOXHOLLOW_MODS_H_

#ifdef __cplusplus
extern "C" {
#endif

void fhModsInit(int argc, char** argv, const char* userPath);
void fhModsUpdate(void);
void fhModsShutdown(void);

#ifdef __cplusplus
}
#endif

#endif /* FOXHOLLOW_MODS_H_ */
