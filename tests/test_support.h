#ifndef FOXHOLLOW_TEST_SUPPORT_H
#define FOXHOLLOW_TEST_SUPPORT_H

#include <stdint.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define FH_CHECK(condition) do { \
    if (!(condition)) { \
        fprintf(stderr, "%s:%d: check failed: %s\n", __FILE__, __LINE__, #condition); \
        exit(EXIT_FAILURE); \
    } \
} while (0)

#define FH_UNEXPECTED_CALL() do { \
    fprintf(stderr, "%s:%d: unexpected call: %s\n", __FILE__, __LINE__, __func__); \
    exit(EXIT_FAILURE); \
} while (0)

#define FH_CHECK_NEAR(actual, expected, tolerance) \
    FH_CHECK(fabs((double)(actual) - (double)(expected)) <= (tolerance))

static inline void fh_test_write_be16(void* destination, uint16_t value) {
    uint8_t* bytes = destination;
    bytes[0] = value >> 8;
    bytes[1] = value;
}

static inline void fh_test_write_be32(void* destination, uint32_t value) {
    uint8_t* bytes = destination;
    bytes[0] = value >> 24;
    bytes[1] = value >> 16;
    bytes[2] = value >> 8;
    bytes[3] = value;
}

static inline void* fh_test_alloc_high(size_t count, size_t size) {
    void* allocation = calloc(count, size);
    FH_CHECK(allocation != NULL);
    FH_CHECK((uintptr_t)allocation > UINT32_MAX);
    return allocation;
}

#endif
