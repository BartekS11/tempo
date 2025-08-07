#pragma once

#include <SDL3/SDL.h>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <math.h>

#define SCREENWIDTH  1280
#define SCREENHEIGHT 720
#define TARGETFPS    61

#define TILESIZE     40
#define HALFTILESIZE (TILESIZE / 2)
#define BASIC_BUFFER_SIZE 8192

#define DEFAULT_FOV  70
#define LINE_THICKNESS 10
#define RAYS_COUNT  1000

#define VELOCITYSTEP 0.002f

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

#define BS_PI M_PI
#define BS_PI_2 M_PI_2
#define BS_PI_4 M_PI_4

#define SQR_VEC(v1, v2) ({ \
    __typeof__(v1) _v1 = (v1); \
    __typeof__(v2) _v2 = (v2); \
    ((_v1.x - _v2.x) * (_v1.x - _v2.x)) + ((_v1.y - _v2.y) * (_v1.y - _v2.y)); \
})

static bool DEBUG_FLAG = false;

typedef uint16_t  bs_uint;
typedef uint32_t  bs_uint32;
typedef int16_t   bs_int;
typedef int32_t   bs_int32;
typedef double    bs_double;
typedef float     bs_float;
typedef unsigned char bs_uchar;

typedef SDL_FPoint bs_Vector2;
typedef SDL_Color bs_Color;
