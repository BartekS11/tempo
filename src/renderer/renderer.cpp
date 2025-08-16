#include "../../include/engine/engine.h"
#include "../../include/g_lib.h"
#include "../../include/game/game.h"
#include "../../include/map.h"
#include "../../include/utils.h"
#include <algorithm>
#include <cmath>
#include <cstdio>

void Render3DMap(bs_Vector2 cameraPosition,
bs_float                    cameraRotation,
bs_int                      lineThickness,
bs_int                      fov,
WindowSettings*             pWindowSettings)
{
    for(bs_int i = -fov; i < fov; i++) {
        bs_int     c   = 0;
        bs_Vector2 hit = {};

        bs_float   rayAngle = (cameraRotation + i) * (BS_PI / 180);
        bs_Vector2 dir      = { sin(rayAngle), cos(rayAngle) };

        StepRay(cameraPosition, dir, RAYS_COUNT, &c, { 130U, 130U, 130U, 255U }, &hit);

        // BSNOTE: Remove fisheye effect by multiplying distance by cos of angle difference
        bs_float angleDiff = rayAngle - (cameraRotation * (BS_PI / 180));
        bs_float dist = sqrtf(SQR_VEC(cameraPosition, hit)) * cosf(angleDiff);

        // BSNOTE: Calculate color based on distance, color fading
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
        SDL_FRect rect = { (bs_float)x, (bs_float)y, (bs_float)lineThickness,
            (bs_float)(RAYS_COUNT / dist) };
        SDL_RenderFillRect(pWindowSettings->pRenderer, &rect);
    }
}

// BSTODO: Add handling of rendering flat map when debug flag will be true
void RenderFlatMap(const bs_int map[MAP_WIDTH * MAP_HEIGHT])
{
    for(size_t row = 0; row < MAP_WIDTH; row++) {
        for(size_t column = 0; column < MAP_HEIGHT; column++) {
            bs_Color std_color = { 0U, 121U, 241U, 255U };
            if(map[row * MAP_WIDTH + column] == 1) {
                std_color = { 102, 191, 255, 255 };
            }
            // SDL_RenderRect(renderer, );
            // DrawRectangle(column * TILESIZE, row * TILESIZE, TILESIZE, TILESIZE, std_color);
        }
    }
}

// BSTODO: Add handling of rendering 2d circle player on the minimap (renderflatmap)
void Render2DPlayer(const bs_Vector2 position, bs_double dt)
{
    // DrawCircle(position.x * TILESIZE + HALFTILESIZE,
    // position.y * TILESIZE + HALFTILESIZE, 6, ORANGE);
}

void Draw(Player* player, bs_double dt, WindowSettings* pWindowSettings)
{
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
