#include "test_support.h"
#include "../game/src/dlls/objects/523_FireFly/FireFly.c"

static int heightQueries;
static int radiusQueries;
static int angleQueries;
static int rotations;
static int expectedRadius;
static f32 expectedY;
static f32 expectedZ;

int randomGetRange(int min, int max) {
    if (min == 0 && max == 60) {
        heightQueries++;
        return 12;
    }
    if (min == 20 && max == expectedRadius) {
        radiusQueries++;
        return 20;
    }
    FH_CHECK(min == 3000 && max == 5000);
    angleQueries++;
    return 3000;
}

void vecRotateZXY(s16* rotation, f32* vector) {
    FH_CHECK(rotation[0] == 0 && rotation[1] == 0 && rotation[2] == 0);
    FH_CHECK(vector[0] == 0 && vector[1] == expectedY && vector[2] == expectedZ);
    rotations++;
}

int main(void) {
    const f32 radii[] = {20.5f, 21.0f, 40.0f};
    for (int first = 0; first <= 1; first++) {
        for (int radiusIndex = 0; radiusIndex < 3; radiusIndex++) {
            // Given a firefly on its first or later target, with radius below, at, or above the cutoff.
            GameObject obj = {0};
            FireFlyFlightState flight = {0};
            flight.firstFrame = first;
            flight.ampMax = 60;
            flight.radius = radii[radiusIndex];
            flight.posX = 100;
            flight.posY = 200;
            flight.posZ = 300;
            flight.angle = -3000;
            expectedRadius = (int)flight.radius;
            expectedY = first ? 60 : 12;
            expectedZ = radiusIndex == 0 ? 0 : flight.radius - 20;
            heightQueries = radiusQueries = angleQueries = rotations = 0;

            // When deterministic random choices select a target with zero final rotation.
            firefly_pickWanderTarget(&obj, &flight);

            // Then the first target uses maximum height, later targets use randomness, and only radii at least 21 spread.
            FH_CHECK(flight.firstFrame == 0 && flight.angle == 0);
            FH_CHECK(flight.targetX == 100 && flight.targetY == 200 + expectedY);
            FH_CHECK(flight.targetZ == 300 + expectedZ);
            FH_CHECK(heightQueries == !first && radiusQueries == (radiusIndex != 0));
            FH_CHECK(angleQueries == 1 && rotations == 1);
        }
    }
    return EXIT_SUCCESS;
}
