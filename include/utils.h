#pragma once

#include "raylib.h"
#include <cstddef>
#include <cstdint>
#include <cstdlib>

#define SCREENWIDTH  1280
#define SCREENHEIGHT 720
#define TARGETFPS    61

#define TILESIZE     40
#define BS_BASIC_BUFFER_SIZE 8192

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

typedef uint16_t  bs_uint;
typedef int16_t   bs_int;
typedef double    bs_double;
typedef float     bs_float;

typedef Vector3 bs_Vec3;

typedef Ray bs_Ray;

typedef Color bs_Color;

typedef struct {
    bs_Vec3 center;
    bs_float radius;
    bs_Color color;
} bs_Sphere;

// TODO: Do i need it at all??
typedef struct {
    bs_Vec3 position;
    bs_Color color;
} bs_Light;


typedef struct {
    Vector2 playerPos;
    bs_int  playerRot;
    Vector2 playerForward;
} Player;

static const float VELOCITYSTEP = 0.002f;