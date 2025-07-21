#include "../../include/g_lib.h"
#include "../../include/raylib.h"
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

        if(DEBUG_FLAG) {
            DrawLine(current.x * TILESIZE + HALFTILESIZE, current.y * TILESIZE + HALFTILESIZE,
            next.x * TILESIZE + HALFTILESIZE, next.y * TILESIZE + HALFTILESIZE, GRAY);
        }
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
WindowSettings*             pWindowSettings)
{
    for(bs_int i = -fov; i < fov; i++) {
        bs_int     c   = 0;
        bs_Vector2 hit = {};

        bs_float   rayAngle = (cameraRotation + i) * (PI / 180);
        bs_Vector2 dir      = { sin(rayAngle), cos(rayAngle) };

        StepRay(cameraPosition, dir, RAYS_COUNT, &c, GRAY, &hit);

        // BS NOTE: Remove fisheye effect by multiplying distance by cos of angle difference
        bs_float angleDiff = rayAngle - (cameraRotation * (PI / 180));
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
        // BS_TRACE("Drawing ray at x: %d, y: %d", pWindowSettings->screenWidth, pWindowSettings->screenHeight);
        DrawRectangle(x, y, lineThickness, (RAYS_COUNT / dist), color);
    }
}

// BS TODO: Make this function that will behavior the same on every fps count
bs_Vector2 Update2DPlayer(bs_Vector2* pPosition, bs_float* rotation, bs_double dt)
{
    bs_Vector2 forward = { sin(*rotation * (PI / 180)), cos(*rotation * (PI / 180)) };

    bs_Vector2 velocity      = { 0, 0 };
    bs_float   rotationSpeed = 0.3f;

    // BS NOTE: Input handling and movement
    if(IsKeyDown(KEY_W)) {
        velocity = { VELOCITYSTEP * forward.x, VELOCITYSTEP * forward.y };
        BS_TRACE("Moving forward, %f, %f", velocity.x, velocity.y);
    }

    if(IsKeyDown(KEY_S)) {
        velocity = { -VELOCITYSTEP * forward.x, -VELOCITYSTEP * forward.y };
        BS_TRACE("Moving backwards, %f, %f", velocity.x, velocity.y);
    }

    if(IsKeyDown(KEY_A)) {
        (*rotation) -= rotationSpeed;
        BS_TRACE("Rotating left %d, ", *rotation);
    }

    if(IsKeyDown(KEY_D)) {
        (*rotation) += rotationSpeed;
        BS_TRACE("Rotating right %d, ", *rotation);
    }

    if(IsKeyDown(KEY_LEFT_SHIFT)) {
        velocity.x *= 2;
        velocity.y *= 2;
        BS_TRACE("Running, %f, %f", velocity.x, velocity.y);
    }

    if(!IsHit(map, { pPosition->x + velocity.x, pPosition->y + velocity.y }, 0.5)) {
        pPosition->x += velocity.x;
        pPosition->y += velocity.y;
    }

    if(IsKeyDown(KEY_ESCAPE)) {
        CloseWindow();
    }
    return forward;
}

void RenderFlatMap(const bs_int map[MAP_WIDTH][MAP_HEIGHT])
{
    for(size_t row = 0; row < MAP_WIDTH; row++) {
        for(size_t column = 0; column < MAP_HEIGHT; column++) {
            bs_Color std_color = BLUE;
            if(map[row][column] == 1) {
                std_color = SKYBLUE;
            }
            DrawRectangle(column * TILESIZE, row * TILESIZE, TILESIZE, TILESIZE, std_color);
        }
    }
}

void Render2DPlayer(const bs_Vector2 position, double dt)
{
    DrawCircle(position.x * TILESIZE + HALFTILESIZE,
    position.y * TILESIZE + HALFTILESIZE, 6, ORANGE);
}

void UpdatePlayer(Player* pPlayer, double dt)
{
    double currentTime = GetTime();

    if(currentTime - dt > 1.0f / 150.0f) {
        dt = currentTime;
        pPlayer->playerForward =
        Update2DPlayer(&pPlayer->playerPos, &pPlayer->playerRot, dt);

        pPlayer->playerForward.x += (0.00001 * dt);
        pPlayer->playerForward.y += (0.00001 * dt);
    }
}

void Draw(Player* player, double dt, WindowSettings* pWindowSettings)
{
    ClearBackground(BLACK);

    Render3DMap(player->playerPos, player->playerRot, LINE_THICKNESS,
    DEFAULT_FOV, pWindowSettings);

    if(DEBUG_FLAG) {
        RenderFlatMap(map);
        Render2DPlayer(player->playerPos, dt);
        DrawFPS(pWindowSettings->screenHeight - 100, 10);
    }
}
