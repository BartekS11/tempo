#include "../../include/engine/engine.h"
#include "../../include/g_lib.h"
#include "../../include/game/game.h"
#include "../../include/utils.h"
#include "../map.cpp"
#include <algorithm>
#include <cmath>
#include <cstdio>

// BS NOTE: Ye i know its scanning full map instead of part, BSP trees and all that stuff CARMAC GENIUS
bool IsHit(const bs_int map[MAP_WIDTH][MAP_HEIGHT], bs_Vector2 point, bs_float size)
{
    for(size_t row = 0; row < MAP_WIDTH; row++) {
        for(size_t col = 0; col < MAP_HEIGHT; col++) {
            if(col < point.x + size && col + size > point.x &&
            row < point.y + size && row + size > point.y && map[row][col] == 1) {
                return true;
            }
        }
    }

    return false;
}

void StepRay(bs_Vector2 position, bs_Vector2 forward, bs_int stepCount, bs_int* pIncr, bs_Color color, bs_Vector2* pHit)
{
    bs_Vector2 dir = forward;

    // BS NOTE Normalize direction
    bs_float len = sqrtf(dir.x * dir.x + dir.y * dir.y);
    if(len == 0) len = 1.0f;
    dir.x /= len;
    dir.y /= len;

    bs_float   stepSize = 0.05f;
    bs_Vector2 current  = position;
    for(bs_int i = 0; i < stepCount; ++i) {
        bs_Vector2 next = { current.x + dir.x * stepSize, current.y + dir.y * stepSize };

        // if(DEBUG_FLAG) {
        //     DrawLine(current.x * TILESIZE + HALFTILESIZE, current.y * TILESIZE + HALFTILESIZE,
        //     next.x * TILESIZE + HALFTILESIZE, next.y * TILESIZE + HALFTILESIZE, GRAY);
        // }
        if(IsHit(map, next, 0.5f)) {
            *pHit  = next;
            *pIncr = 0;
            return;
        }
        current = next;
    }
    *pHit  = current;
    *pIncr = 0;
}

void Render3DMap(bs_Vector2 cameraPosition,
bs_float                    cameraRotation,
bs_int                      lineThickness,
bs_int                      fov,
WindowSettingsV2*           pWindowSettings)
{
    for(bs_int i = -fov; i < fov; i++) {
        bs_int     c   = 0;
        bs_Vector2 hit = {};

        bs_float   rayAngle = (cameraRotation + i) * (BS_PI / 180);
        bs_Vector2 dir      = { sin(rayAngle), cos(rayAngle) };

        StepRay(cameraPosition, dir, RAYS_COUNT, &c, { 130U, 130U, 130U, 255U }, &hit);

        // BS NOTE: Remove fisheye effect by multiplying distance by cos of angle difference
        bs_float angleDiff = rayAngle - (cameraRotation * (BS_PI / 180));
        bs_float dist = sqrtf(SQR_VEC(cameraPosition, hit)) * cosf(angleDiff);

        // BS NOTE: Calculate color based on distance, color fading
        bs_uint32 hexColor = 0xFF0000FF;
        bs_uchar  r        = (hexColor >> 16) & 0xFF;
        bs_uchar  g        = (hexColor >> 8) & 0xFF;
        bs_uchar  b        = hexColor & 0xFF;

        bs_float fade = 1.0f - dist * 0.3f;
        if(fade < 0.2f) fade = 0.2f;

        r = (bs_uchar)(r * fade);
        g = (bs_uchar)(g * fade);
        b = (bs_uchar)(b * fade);

        bs_Color color = { r, g, b, 255 };
        bs_int   x = (i * lineThickness) + (pWindowSettings->screenWidth / 2);
        bs_int y = (pWindowSettings->screenHeight / 2) - (RAYS_COUNT / dist) / 2;

        SDL_SetRenderDrawColor(
        pWindowSettings->pRenderer, color.r, color.g, color.b, color.a);
        SDL_FRect rect = { (float)x, (float)y, (float)lineThickness,
            (float)(RAYS_COUNT / dist) };
        SDL_RenderFillRect(pWindowSettings->pRenderer, &rect);
    }
}

// BS TODO: Make this function that will behavior the same on every fps count
bs_Vector2 Update2DPlayer(bs_Vector2* pPosition, bs_double* pRotation, float dt)
{
    const bool* state = SDL_GetKeyboardState(NULL);

    bs_Vector2 forward       = { (float)sin(*pRotation * (BS_PI / 180)),
              (float)cos(*pRotation * (BS_PI / 180)) };
    bs_Vector2 velocity      = { 0, 0 };
    float      rotationSpeed = .3f;

    if(state[SDL_SCANCODE_W]) {
        velocity.x += VELOCITYSTEP * forward.x;
        velocity.y += VELOCITYSTEP * forward.y;
    }
    if(state[SDL_SCANCODE_S]) {
        velocity.x -= VELOCITYSTEP * forward.x;
        velocity.y -= VELOCITYSTEP * forward.y;
    }
    if(state[SDL_SCANCODE_A]) {
        *pRotation -= rotationSpeed;
    }
    if(state[SDL_SCANCODE_D]) {
        *pRotation += rotationSpeed;
    }
    if(state[SDL_SCANCODE_LSHIFT]) {
        velocity.x *= 2;
        velocity.y *= 2;
    }

    if(!IsHit(map, { pPosition->x + velocity.x, pPosition->y + velocity.y }, 0.5)) {
        pPosition->x += velocity.x;
        pPosition->y += velocity.y;
    }
    return forward;
}

void RenderFlatMap(const bs_int map[MAP_WIDTH][MAP_HEIGHT])
{
    for(size_t row = 0; row < MAP_WIDTH; row++) {
        for(size_t column = 0; column < MAP_HEIGHT; column++) {
            bs_Color std_color = { 0U, 121U, 241U, 255U };
            if(map[row][column] == 1) {
                std_color = { 102, 191, 255, 255 };
            }
            // SDL_RenderRect(renderer, );
            // DrawRectangle(column * TILESIZE, row * TILESIZE, TILESIZE, TILESIZE, std_color);
        }
    }
}

void Render2DPlayer(const bs_Vector2 position, double dt)
{
    // DrawCircle(position.x * TILESIZE + HALFTILESIZE,
    // position.y * TILESIZE + HALFTILESIZE, 6, ORANGE);
}

void UpdatePlayer(Player* pPlayer, double dt, SDL_Event* pEvent)
{
    double currentTime = SDL_GetTicks();

    dt = currentTime;
    pPlayer->playerForward = Update2DPlayer(&pPlayer->playerPos, &pPlayer->playerRot, dt);

    pPlayer->playerForward.x += (0.00001 * dt);
    pPlayer->playerForward.y += (0.00001 * dt);
}

void Draw(Player* player, double dt, WindowSettingsV2* pWindowSettings)
{
    // ClearBackground(BLACK);
    SDL_SetRenderDrawColor(pWindowSettings->pRenderer, 0, 0, 0, 255);
    SDL_RenderClear(pWindowSettings->pRenderer);

    Render3DMap(player->playerPos, player->playerRot, LINE_THICKNESS,
    DEFAULT_FOV, pWindowSettings);

    if(DEBUG_FLAG) {
        RenderFlatMap(map);
        // Render2DPlayer(player->playerPos, dt);
        // DrawFPS(pWindowSettings->screenHeight - 100, 10);
    }
    SDL_RenderPresent(pWindowSettings->pRenderer);
}
