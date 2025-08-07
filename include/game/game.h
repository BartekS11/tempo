#pragma once

#include "../utils.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_init.h>

typedef struct {
    bs_Vector2 playerPos;
    bs_double  playerRot;
    bs_Vector2 playerForward;
} Player;

static Player player{ .playerPos = { 1, 1 }, .playerRot = 0, .playerForward = { 0, 0 } };
