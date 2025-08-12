#pragma once

#include "../g_lib.h"
#include "../map.h"
#include "../utils.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_init.h>

typedef struct {
    bs_Vector2 playerPos;
    bs_double  playerRot;
    bs_Vector2 playerForward;
} Player;

static Player player{ .playerPos = { 1, 1 }, .playerRot = 0, .playerForward = { 0, 0 } };

/**
 * BSNOTE: Definition of procedures
 */
void Run(void);

bs_Vector2 Update2DPlayer(bs_Vector2* pPosition, bs_double* pRotation, bs_float dt);

void StepRay(bs_Vector2 position,
bs_Vector2              forward,
bs_int                  stepCount,
bs_int*                 pIncr,
bs_Color                color,
bs_Vector2*             pHit);

bool IsHit(const bs_int map[MAP_WIDTH][MAP_HEIGHT], bs_Vector2 point, bs_float size);

void UpdatePlayer(Player* pPlayer, double dt, SDL_Event* pEvent);