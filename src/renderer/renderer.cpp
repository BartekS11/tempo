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
    for(uint8_t row = 0; row < MAP_WIDTH; row++) {
        for(uint8_t col = 0; col < MAP_HEIGHT; col++) {
            if(col < point.x + size && col + size > point.x &&
            row < point.y + size && row + size > point.y && map[row][col] == 1) {
                return true;
            }
        }
    }

    return false;
}

void StepRay(bs_Vector2 position, bs_Vector2 forward, bs_int stepCount, bs_int* incr, Color color, bs_Vector2* pHit)
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
            *pHit = next;
            *incr = 0;
            return;
        }
        current = next;
    }
    *pHit = current;
    *incr = 0;
}

void Render3DMap(bs_Vector2 cameraPosition, bs_float cameraRotation, bs_int lineThickness, bs_int fov)
{
    for(bs_int i = -fov; i < fov; i++) {
        bs_int  c   = 0;
        bs_Vector2 hit = {};

        bs_float   rayAngle = (cameraRotation + i) * (PI / 180);
        bs_Vector2 dir      = { sin(rayAngle), cos(rayAngle) };

        StepRay(cameraPosition, dir, RAYS_COUNT, &c, GRAY, &hit);

        // BS NOTE: Remove fisheye effect by multiplying distance by cos of angle difference
        bs_float angleDiff = (i * (PI / 180));
        bs_float dist      = sqrtf(SQR_VEC(cameraPosition, hit)) * cosf(angleDiff);

        bs_Color color = { (unsigned char)(150 - dist * 1.5),
            (unsigned char)(150 - dist * 1.5), (unsigned char)(150 - dist * 1.5), 255 };

        DrawRectangle((i * lineThickness + (lineThickness * fov / 2) + (10 * TILESIZE) - 60),
        8 * TILESIZE - (RAYS_COUNT / dist) / 2, lineThickness, (RAYS_COUNT / dist), color);
    }
}

// BS TODO: Make this function that will behavior the same on every fps count
bs_Vector2 Update2DPlayer(bs_Vector2* position, bs_float* rotation, double dt)
{
    bs_Vector2 forward =
    (bs_Vector2){ sin(*rotation * (PI / 180)), cos(*rotation * (PI / 180)) };

    bs_Vector2 velocity = (bs_Vector2){ 0, 0 };

    // BS NOTE: Input handling and movement
    if(IsKeyDown(KEY_W)) {
        velocity = (bs_Vector2){ VELOCITYSTEP * forward.x, VELOCITYSTEP * forward.y };
        BS_TRACE("Moving forward, %f, %f", velocity.x, velocity.y);
    }

    if(IsKeyDown(KEY_S)) {
        velocity = (bs_Vector2){ -VELOCITYSTEP * forward.x, -VELOCITYSTEP * forward.y };
        BS_TRACE("Moving backwards, %f, %f", velocity.x, velocity.y);
    }

    if(IsKeyDown(KEY_A)) {
        (*rotation) -= 0.3f;
        BS_TRACE("Rotating left %d, ", *rotation);
    }

    if(IsKeyDown(KEY_D)) {
        (*rotation) += 0.3f;
        BS_TRACE("Rotating right %d, ", *rotation);
    }


    if(IsKeyDown(KEY_LEFT_SHIFT)) {
        velocity.x *= 2;
        velocity.y *= 2;
        BS_TRACE("Running, %f, %f", velocity.x, velocity.y);
    }

    if(!IsHit(map, (bs_Vector2){ position->x + velocity.x, position->y + velocity.y }, 0.5)) {
        position->x += velocity.x;
        position->y += velocity.y;
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

void UpdatePlayer(Player* player, double dt)
{
    double currentTime = GetTime();

    if(currentTime - dt > 1.0f / 150.0f) {
        dt = currentTime;
        player->playerForward =
        Update2DPlayer(&player->playerPos, &player->playerRot, dt);

        player->playerForward.x += (0.00001 * dt);
        player->playerForward.y += (0.00001 * dt);
    }
}

void Draw(Player* player, double dt)
{
    ClearBackground(BLACK);

    Render3DMap(player->playerPos, player->playerRot, LINE_THICKNESS, DEFAULT_FOV);

    if(DEBUG_FLAG) {
        RenderFlatMap(map);
        Render2DPlayer(player->playerPos, dt);
        DrawFPS(1000, 10);
    }
}
