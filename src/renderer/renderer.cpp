#include "../../include/g_lib.h"
#include "../../include/raylib.h"
#include "../../include/utils.h"
#include "../map.cpp"
#include <algorithm>
#include <cmath>
#include <cstdio>

// BS NOTE: Ye i know its scanning full map instead of part, BSP trees and all that stuff CARMAC GENIUS
bool IsHit(const bs_int map[10][10], Vector2 point, float size) {
    for(uint8_t row = 0; row < 10; row++) {
        for(uint8_t col = 0; col < 10; col++) {
            if(col < point.x + size && col + size > point.x &&
            row < point.y + size && row + size > point.y && map[row][col] == 1) {
                return true;
            }
        }
    }

    return false;
}

void StepRay(const Vector2 position, Vector2 forward, bs_int stepCount, bs_int* incr, Color color, Vector2* pHit) {
    Vector2 dir = forward;
    // BS NOTE Normalize direction
    float len = sqrtf(dir.x * dir.x + dir.y * dir.y);
    if(len == 0) len = 1.0f;
    dir.x /= len;
    dir.y /= len;

    float   stepSize = 0.05f;
    Vector2 current  = position;
    for(bs_int i = 0; i < stepCount; ++i) {
        Vector2 next = { current.x + dir.x * stepSize, current.y + dir.y * stepSize };

        if(DEBUG_FLAG)
            DrawLine(current.x * TILESIZE + HALFTILESIZE, current.y * TILESIZE + HALFTILESIZE, next.x * TILESIZE + HALFTILESIZE, next.y * TILESIZE + HALFTILESIZE, GRAY);

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


float Dist(const Vector2 v1, const Vector2 v2) {
    return ((v1.x - v2.x) * (v1.x - v2.x)) + ((v1.y - v2.y) * (v1.y - v2.y));
}

void Render3DMap(Vector2 cameraPosition, float cameraRotation, bs_int lineThickness, bs_int fov) {
    for(bs_int i = -fov; i < fov; i++) {
        bs_int  c   = 0;
        Vector2 hit = {};

        float   rayAngle = (cameraRotation + i) * (PI / 180);
        Vector2 dir      = { sin(rayAngle), cos(rayAngle) };

        StepRay(cameraPosition, dir, RAYS_COUNT, &c, GRAY, &hit);

        // BS: Remove fisheye effect by multiplying distance by cos of angle difference
        float angleDiff = (i * (PI / 180));
        float dist      = sqrtf(Dist(cameraPosition, hit)) * cosf(angleDiff);

        Color color = { (unsigned char)(150 - dist * 1.5),
            (unsigned char)(150 - dist * 1.5), (unsigned char)(150 - dist * 1.5), 255 };

        DrawRectangle((i * lineThickness + (lineThickness * fov / 2) + (10 * TILESIZE) - 60),
        8 * TILESIZE - (RAYS_COUNT / dist) / 2, lineThickness, (RAYS_COUNT / dist), color);
    }
}

// BS TODO: Make this function that will behavior the same on every fps count
Vector2 Update2DPlayer(Vector2* position, float* rotation, double dt) {
    Vector2 forward =
    (Vector2){ sin(*rotation * (PI / 180)), cos(*rotation * (PI / 180)) };

    Vector2 velocity = (Vector2){ 0, 0 };

    // BS NOTE: Input handling and movement
    if(IsKeyDown(KEY_W)) {
        velocity = (Vector2){ VELOCITYSTEP * forward.x, VELOCITYSTEP * forward.y };
        BS_TRACE("Moving forward, %f, %f", velocity.x, velocity.y);
    }

    if(IsKeyDown(KEY_S)) {
        velocity = (Vector2){ -VELOCITYSTEP * forward.x, -VELOCITYSTEP * forward.y };
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
    if(IsKeyDown(KEY_ESCAPE)) {
        CloseWindow();
    }
    if(IsKeyDown(KEY_LEFT_SHIFT)) {
        velocity.x *= 2;
        velocity.y *= 2;
        BS_TRACE("Running, %f, %f", velocity.x, velocity.y);
    }

    if(!IsHit(map, (Vector2){ position->x + velocity.x, position->y + velocity.y }, 0.5)) {
        position->x += velocity.x;
        position->y += velocity.y;
    }

    return forward;
}

void RenderFlatMap(const bs_int map[MAP_WIDTH][MAP_HEIGHT]) {
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

void Render2DPlayer(const Vector2 position, double dt) {
    DrawCircle(position.x * TILESIZE + HALFTILESIZE,
    position.y * TILESIZE + HALFTILESIZE, 6, ORANGE);
}

void UpdatePlayer(Player* player, double dt) {
    double currentTime = GetTime();

    if(currentTime - dt > 1.0f / 150.0f) {
        dt = currentTime;
        player->playerForward =
        Update2DPlayer(&player->playerPos, &player->playerRot, dt);

        player->playerForward.x += (0.00001 * dt);
        player->playerForward.y += (0.00001 * dt);
    }
}

void Draw(Player* player, double dt) {
    ClearBackground(BLACK);

    Render3DMap(player->playerPos, player->playerRot, LINE_THICKNESS, DEFAULT_FOV);

    if(DEBUG_FLAG) {
        RenderFlatMap(map);
        Render2DPlayer(player->playerPos, dt);
        DrawFPS(1000, 10);
    }
}
