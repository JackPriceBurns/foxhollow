#ifndef DLLS_OBJECTS_373_DFROPENODE_H_
#define DLLS_OBJECTS_373_DFROPENODE_H_

#include "dlls/object_descriptor.h"
#include "dolphin/mtx/vec_types.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

#define DFROPENODE_OBJECT_GROUP 0x17

typedef enum DFropenodeStyle {
    DFROPENODE_STYLE_ROPE,
    DFROPENODE_STYLE_WATER,
    DFROPENODE_STYLE_COUNT
} DFropenodeStyle;

typedef struct DFropenodePlacement {
    ObjPlacement base;
    u8 nodeId; /* odd IDs start a rope; the following even ID ends it */
    u8 pad19[0x1B - 0x19];
    u8 style; /* DFropenodeStyle */
    s16 fadeGameBit;
    u8 pad1E[0x20 - 0x1E];
} DFropenodePlacement;

typedef struct DFropenodeRopeNode {
    Vec pos;
    Vec velocity;
    Vec force;
    u8 linkCount;
    u8 pad25[3];
    struct DFropenodeRopeLink* links[2];
    u8 locked;
    u8 pad31[3];
} DFropenodeRopeNode;

typedef struct DFropenodeRopeLink {
    f32 length;
    DFropenodeRopeNode* a;
    DFropenodeRopeNode* b;
    f32 restLength;
    f32 stiffness;
    f32 maxLength;
    Vec force;
} DFropenodeRopeLink;

typedef struct DFropenodeRope {
    DFropenodeRopeNode* nodes;
    DFropenodeRopeLink* links;
    u8 count;
    u8 pad09[3];
    Vec start;
    Vec end;
    f32 totalLength;
    s32 solverIterations;
    f32 maxForce;
    f32 timeStep;
    s8 sway;
    s8 direction;
    u8 pad36[2];
    f32 damping;
    f32 gravityOverMass;
    f32 stepOverMass;
} DFropenodeRope;

typedef struct DFropenodePlaneEquation {
    Vec normal;
    f32 distance;
} DFropenodePlaneEquation;

typedef struct DFropenodeState {
    GameObject* linkedNode;
    f32 boundsMinX;
    f32 boundsMaxX;
    f32 boundsMinZ;
    f32 boundsMaxZ;
    f32 minimumY;
    s16 ropeYaw;
    u8 pad1A[2];
    DFropenodePlaneEquation plane;
    DFropenodeRope* rope;
    u8 hidden : 1;
    u8 pad30 : 7;
    u8 pad31[3];
} DFropenodeState;

typedef struct DFropenodeRenderState {
    u8 red;
    u8 green;
    u8 blue;
} DFropenodeRenderState;

typedef struct DFropenodeInterface {
    ObjectInterface object;
    void (*getPlaneEquation)(GameObject* obj, DFropenodePlaneEquation* out);
    void (*getWorldPosAtPhase)(f32 phase, GameObject* obj, f32* xOut, f32* yOut, f32* zOut);
    void (*advancePhaseByDistance)(GameObject* obj, f32* phase, f32 distance);
    void (*applyForceAtPhase)(f32 phase, f32 force, GameObject* obj);
    int (*findNearestRopePoint)(GameObject* obj, f32 worldX, f32 worldY, f32 worldZ, f32* distanceOut, f32* phaseOut,
                                u8* sideOut);
    int (*getAngle)(GameObject* obj);
    void (*setVisible)(GameObject* obj, int visible);
    int (*isVisible)(GameObject* obj);
    void (*setMinY)(GameObject* obj, f32 value);
    void (*clearLinkedObj)(GameObject* obj);
} DFropenodeInterface;

STATIC_ASSERT(offsetof(DFropenodePlacement, base) == 0x00);
STATIC_ASSERT(offsetof(DFropenodePlacement, nodeId) == 0x18);
STATIC_ASSERT(offsetof(DFropenodePlacement, pad19) == 0x19);
STATIC_ASSERT(offsetof(DFropenodePlacement, style) == 0x1B);
STATIC_ASSERT(offsetof(DFropenodePlacement, fadeGameBit) == 0x1C);
STATIC_ASSERT(offsetof(DFropenodePlacement, pad1E) == 0x1E);
STATIC_ASSERT(sizeof(DFropenodePlacement) == 0x20);

STATIC_ASSERT(offsetof(DFropenodeRopeNode, pos) == 0x00);
STATIC_ASSERT(offsetof(DFropenodeRopeNode, velocity) == 0x0C);
STATIC_ASSERT(offsetof(DFropenodeRopeNode, force) == 0x18);
STATIC_ASSERT(offsetof(DFropenodeRopeNode, linkCount) == 0x24);
STATIC_ASSERT(offsetof(DFropenodeRopeNode, links) == 0x28);
STATIC_ASSERT(offsetof(DFropenodeRopeNode, locked) == 0x30);
STATIC_ASSERT(sizeof(DFropenodeRopeNode) == 0x34);

STATIC_ASSERT(offsetof(DFropenodeRopeLink, length) == 0x00);
STATIC_ASSERT(offsetof(DFropenodeRopeLink, a) == 0x04);
STATIC_ASSERT(offsetof(DFropenodeRopeLink, b) == 0x08);
STATIC_ASSERT(offsetof(DFropenodeRopeLink, restLength) == 0x0C);
STATIC_ASSERT(offsetof(DFropenodeRopeLink, stiffness) == 0x10);
STATIC_ASSERT(offsetof(DFropenodeRopeLink, maxLength) == 0x14);
STATIC_ASSERT(offsetof(DFropenodeRopeLink, force) == 0x18);
STATIC_ASSERT(sizeof(DFropenodeRopeLink) == 0x24);

STATIC_ASSERT(offsetof(DFropenodeRope, nodes) == 0x00);
STATIC_ASSERT(offsetof(DFropenodeRope, links) == 0x04);
STATIC_ASSERT(offsetof(DFropenodeRope, count) == 0x08);
STATIC_ASSERT(offsetof(DFropenodeRope, start) == 0x0C);
STATIC_ASSERT(offsetof(DFropenodeRope, end) == 0x18);
STATIC_ASSERT(offsetof(DFropenodeRope, totalLength) == 0x24);
STATIC_ASSERT(offsetof(DFropenodeRope, solverIterations) == 0x28);
STATIC_ASSERT(offsetof(DFropenodeRope, maxForce) == 0x2C);
STATIC_ASSERT(offsetof(DFropenodeRope, timeStep) == 0x30);
STATIC_ASSERT(offsetof(DFropenodeRope, sway) == 0x34);
STATIC_ASSERT(offsetof(DFropenodeRope, direction) == 0x35);
STATIC_ASSERT(offsetof(DFropenodeRope, damping) == 0x38);
STATIC_ASSERT(offsetof(DFropenodeRope, gravityOverMass) == 0x3C);
STATIC_ASSERT(offsetof(DFropenodeRope, stepOverMass) == 0x40);
STATIC_ASSERT(sizeof(DFropenodeRope) == 0x44);

STATIC_ASSERT(offsetof(DFropenodePlaneEquation, normal) == 0x00);
STATIC_ASSERT(offsetof(DFropenodePlaneEquation, distance) == 0x0C);
STATIC_ASSERT(sizeof(DFropenodePlaneEquation) == 0x10);

STATIC_ASSERT(offsetof(DFropenodeState, linkedNode) == 0x00);
STATIC_ASSERT(offsetof(DFropenodeState, boundsMinX) == 0x04);
STATIC_ASSERT(offsetof(DFropenodeState, boundsMaxX) == 0x08);
STATIC_ASSERT(offsetof(DFropenodeState, boundsMinZ) == 0x0C);
STATIC_ASSERT(offsetof(DFropenodeState, boundsMaxZ) == 0x10);
STATIC_ASSERT(offsetof(DFropenodeState, minimumY) == 0x14);
STATIC_ASSERT(offsetof(DFropenodeState, ropeYaw) == 0x18);
STATIC_ASSERT(offsetof(DFropenodeState, pad1A) == 0x1A);
STATIC_ASSERT(offsetof(DFropenodeState, plane) == 0x1C);
STATIC_ASSERT(offsetof(DFropenodeState, rope) == 0x2C);
STATIC_ASSERT(offsetof(DFropenodeState, pad31) == 0x31);
STATIC_ASSERT(sizeof(DFropenodeState) == 0x34);

STATIC_ASSERT(offsetof(DFropenodeRenderState, red) == 0x00);
STATIC_ASSERT(offsetof(DFropenodeRenderState, green) == 0x01);
STATIC_ASSERT(offsetof(DFropenodeRenderState, blue) == 0x02);
STATIC_ASSERT(sizeof(DFropenodeRenderState) == 0x03);

STATIC_ASSERT(offsetof(DFropenodeInterface, object) == 0x00);
STATIC_ASSERT(offsetof(DFropenodeInterface, getPlaneEquation) == 0x20);
STATIC_ASSERT(offsetof(DFropenodeInterface, getWorldPosAtPhase) == 0x24);
STATIC_ASSERT(offsetof(DFropenodeInterface, advancePhaseByDistance) == 0x28);
STATIC_ASSERT(offsetof(DFropenodeInterface, applyForceAtPhase) == 0x2C);
STATIC_ASSERT(offsetof(DFropenodeInterface, findNearestRopePoint) == 0x30);
STATIC_ASSERT(offsetof(DFropenodeInterface, getAngle) == 0x34);
STATIC_ASSERT(offsetof(DFropenodeInterface, setVisible) == 0x38);
STATIC_ASSERT(offsetof(DFropenodeInterface, isVisible) == 0x3C);
STATIC_ASSERT(offsetof(DFropenodeInterface, setMinY) == 0x40);
STATIC_ASSERT(offsetof(DFropenodeInterface, clearLinkedObj) == 0x44);
STATIC_ASSERT(sizeof(DFropenodeInterface) == 0x48);

void DFropenode_updateRopeSimulation(DFropenodeRope* rope);
void DFropenode_setMinY(GameObject* obj, f32 value);
s16 DFropenode_isVisible(GameObject* obj);
void DFropenode_setVisible(GameObject* obj, int visible);
s16 DFropenode_getAngle(GameObject* obj);
void DFropenode_clearLinkedObj(GameObject* obj);
int DFropenode_findNearestRopePoint(GameObject* obj, f32 worldX, f32 worldY, f32 worldZ, f32* distanceOut,
                                    f32* phaseOut, u8* sideOut);
void DFropenode_applyForceAtPhase(f32 phase, f32 force, GameObject* obj);
void DFropenode_advancePhaseByDistance(GameObject* obj, f32* phase, f32 distance);
void DFropenode_getWorldPosAtPhase(f32 phase, GameObject* obj, f32* xOut, f32* yOut, f32* zOut);
void DFropenode_getPlaneEquation(GameObject* obj, DFropenodePlaneEquation* out);
int DFropenode_syncRopeToEndpoints(GameObject* obj);
int DFropenode_getExtraSize(void);
int DFropenode_getObjectTypeId(void);
void DFropenode_free(GameObject* obj);
void DFropenode_render(GameObject* obj, int gdl, int mtxs);
void DFropenode_hitDetect(void);
void DFropenode_update(GameObject* obj);
void DFropenode_init(GameObject* obj, DFropenodePlacement* placement);
void DFropenode_release(void);
void DFropenode_initialise(void);

extern ObjectDescriptor20 gDFropenodeObjDescriptor;

#endif /* DLLS_OBJECTS_373_DFROPENODE_H_ */
