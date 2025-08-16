#pragma once

#include "../g_lib.h"
#include "../map.h"
#include "../utils.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_init.h>

// #define FPS_CAP

typedef struct {

} Game;

typedef struct {
    bs_Vector2 playerPos;
    bs_Vector2 playerForward;
    bs_double  playerRot;
} Player;

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

bool IsHit(const bs_int map[MAP_WIDTH * MAP_HEIGHT], bs_Vector2 point, bs_float size);

void UpdatePlayer(Player* pPlayer, bs_double dt, SDL_Event* pEvent);