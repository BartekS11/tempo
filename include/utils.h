#pragma once

#include "raylib.h"
#include <cstddef>
#include <cstdint>
#include <cstdlib>

#define SCREENWIDTH  1280
#define SCREENHEIGHT 720
#define TARGETFPS    61

#define TILESIZE     40
#define HALFTILESIZE (TILESIZE / 2)
#define BASIC_BUFFER_SIZE 8192

#define DEFAULT_FOV  70
#define LINE_THICKNESS 10
#define RAYS_COUNT  1000

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

#define SQR_VEC(v1, v2) ({ \
    __typeof__(v1) _v1 = (v1); \
    __typeof__(v2) _v2 = (v2); \
    ((_v1.x - _v2.x) * (_v1.x - _v2.x)) + ((_v1.y - _v2.y) * (_v1.y - _v2.y)); \
})

static bool DEBUG_FLAG = false;

typedef uint16_t  bs_uint;
typedef int16_t   bs_int;
typedef double    bs_double;
typedef float     bs_float;

typedef Vector2 bs_Vector2;
typedef Color bs_Color;

typedef struct {
    bs_Vector2 playerPos;
    float playerRot;
    bs_Vector2 playerForward;
} Player;

static const float VELOCITYSTEP = 0.002f;