#include "test_support.h"
#include "../game/src/dlls/objects/205_IceBall/IceBall.c"

f32 timeDelta = 1;
static GameObject* ball;
static GameObject* owner;
static GameObject* objects[3];
static int particles;
static int expectedParticle;
static int notifications;
static int notifiedSlot;
static int sounds;
static int shakes;
static int offsets;
static int frees;
static int moves;
static int enables;
static int volumes;
static int radii;

GameObject** ObjList_GetObjects(int* start, int* count) {
    *start = 1;
    *count = 3;
    return objects;
}

static void notify8(GameObject* obj, int message) {
    FH_CHECK(obj == owner && message == 0x80);
    notifiedSlot = 8;
    notifications++;
}

static void notify9(GameObject* obj, int message) {
    FH_CHECK(obj == owner && message == 0x80);
    notifiedSlot = 9;
    notifications++;
}

static void notify9WithArg(GameObject* obj, int message, int arg) {
    FH_CHECK(arg == 0);
    notify9(obj, message);
}

static int spawn(GameObject* obj, int effect, PartFxSpawnParams* params, u32 mode, u8 model, void* extra) {
    FH_CHECK(obj == ball && effect == expectedParticle);
    FH_CHECK(params == NULL && mode == 1 && model == 255 && extra == NULL);
    particles++;
    return 0;
}

static EffectInterface effects = {.spawnObject = spawn};
static EffectInterface* effectPointer = &effects;
EffectInterface** gPartfxInterface = &effectPointer;

void Sfx_PlayFromObject(GameObject* obj, u16 sound) {
    FH_CHECK(obj == ball && sound == 0x26a);
    sounds++;
}

void CameraShake_Enable(void) { shakes++; }
void CameraShake_SetOffset(f32 offset) { FH_CHECK(offset == 1); offsets++; }
void Obj_FreeObject(GameObject* obj) { FH_CHECK(obj == ball); frees++; }
GameObject* Obj_GetPlayerObject(void) { return objects[1]; }
GameObject* getTrickyObject(void) { return NULL; }

int objMove(GameObject* obj, f32 x, f32 y, f32 z) {
    FH_CHECK(obj == ball && x == 2 && z == 3);
    FH_CHECK_NEAR(y, -0.0679, 0.000001);
    moves++;
    return 0;
}

void ObjHits_EnableObject(GameObject* obj) { FH_CHECK(obj == ball); enables++; }

void ObjHits_SetHitVolumeSlot(ObjAnimComponent* obj, int slot, int type, int source) {
    FH_CHECK(obj == &ball->anim && slot == 10 && type == 1 && source == 0);
    volumes++;
}

void ObjHitbox_SetSphereRadius(ObjAnimComponent* obj, s16 radius) {
    FH_CHECK(obj == &ball->anim && radius == 5);
    radii++;
}

int main(void) {
    ball = fh_test_alloc_high(1, sizeof(*ball));
    owner = fh_test_alloc_high(1, sizeof(*owner));
    GameObject other = {0};
    void* callbacks[10] = {0};
    ObjectInterface* interface = (ObjectInterface*)callbacks;
    owner->anim.dll = &interface;
    callbacks[8] = notify8;
    const int variants[] = {0x2cb, 100, 0x30a};

    for (int variant = 0; variant < 3; variant++) {
        for (int presence = 0; presence < 3; presence++) {
            // Given each ice-ball variant with a live, excluded, or absent high-address owner.
            ball->anim.romDefNo = variants[variant];
            ball->ownerObj = presence == 2 ? NULL : owner;
            objects[0] = owner;
            objects[1] = &other;
            objects[2] = presence == 0 ? owner : &other;
            callbacks[9] = variant == 2 ? (void*)notify9WithArg : (void*)notify9;
            expectedParticle = variant == 0 ? 832 : 835;
            particles = notifications = sounds = shakes = offsets = 0;

            // When the projectile hits a character.
            iceBall_handleCharacterImpact(ball);

            // Then only an owner inside the active list receives the variant's callback, while all impacts emit 25 particles.
            FH_CHECK(notifications == (presence == 0));
            if (notifications) {
                FH_CHECK(notifiedSlot == (variant == 0 ? 8 : 9));
            }
            FH_CHECK(particles == 25 && sounds == 1 && shakes == 1 && offsets == 1);
        }

        // Given the same variant striking a surface instead of a character.
        ball->ownerObj = owner;
        expectedParticle = variant == 0 ? 834 : 836;
        particles = notifications = sounds = shakes = offsets = 0;

        // When the surface impact is handled.
        iceBall_handleSurfaceImpact(ball);

        // Then it uses the surface particles and never sends an owner notification.
        FH_CHECK(particles == 25 && notifications == 0 && sounds == 1 && shakes == 1 && offsets == 1);
    }

    // Given a visible projectile simultaneously reporting player and surface contact.
    ObjHitsPriorityState hits = {0};
    ball->anim.hitReactState = (ObjHitReactState*)&hits;
    objects[1] = owner;
    hits.lastHitObject = (uintptr_t)owner;
    hits.contactFlags = 1;
    hits.flags = 5;
    ball->anim.romDefNo = 0x2cb;
    ball->ownerObj = NULL;
    ball->anim.alpha = 255;
    ball->userData1 = 180;
    ball->anim.velocityX = 2;
    ball->anim.velocityZ = 3;
    expectedParticle = 832;
    particles = sounds = shakes = offsets = 0;

    // When its flight update processes both contacts.
    IceBall_update(ball);

    // Then the character impact wins, the projectile hides for 120 frames, and collision becomes inactive.
    FH_CHECK(ball->anim.alpha == 0 && ball->userData1 == 120 && hits.flags == 4);
    FH_CHECK(particles == 25 && sounds == 1 && shakes == 1 && offsets == 1);
    FH_CHECK(moves == 1 && enables == 1 && volumes == 1 && radii == 1 && frees == 0);

    // When the hidden projectile reaches exactly zero lifetime.
    ball->userData1 = 1;
    IceBall_update(ball);

    // Then retail keeps it alive at zero without repeating motion or impact effects.
    FH_CHECK(ball->userData1 == 0 && frees == 0 && moves == 1 && particles == 25);

    // When one more frame makes that lifetime negative.
    IceBall_update(ball);

    // Then the projectile is freed once.
    FH_CHECK(ball->userData1 == -1 && frees == 1 && particles == 25);
    free(owner);
    free(ball);
    return EXIT_SUCCESS;
}
