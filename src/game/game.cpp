#include "../../include/game/game.h"
// #include "../engine/engine.cpp"
#include "../../include/engine/engine.h"
#include "../../include/renderer/renderer.h"

void Run(void)
{
    bool isRunning = true;

    bs_int32 last_tick = SDL_GetTicks();

    Init();

    while(isRunning) {
        bs_int32 current_tick = SDL_GetTicks();
        bs_float delta_time   = (current_tick - last_tick) / 1000.0f;
        last_tick             = current_tick;
        SDL_Event event;
        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_EVENT_KEY_DOWN) {
                if(event.key.key == SDLK_ESCAPE) {
                    isRunning = false;
                }
            }
            if(event.type == SDL_EVENT_WINDOW_RESIZED) {
                bs_int32 new_width  = event.window.data1;
                bs_int32 new_height = event.window.data2;
                BS_TRACE("Window resized to %dx%d", new_height, new_height);
                SetWindowSettings(GetWindowSettings()->title, new_width, new_height);
            }
            if(event.type == SDL_EVENT_QUIT) {
                isRunning = false;
            }
        }
        // if(IsKeyPressed(KEY_F5)) {
        // DEBUG_FLAG = !DEBUG_FLAG;
        // }

        UpdatePlayer(&player, delta_time, &event);

        Draw(&player, delta_time, GetWindowSettings());
    }


    Shutdown();
}

// BSNOTE: Ye i know its scanning full map instead of part, BSP trees and all that stuff CARMAC GENIUS
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

    // BSNOTE Normalize direction
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

bs_Vector2 Update2DPlayer(bs_Vector2* pPosition, bs_double* pRotation, bs_float dt)
{
    const bool* state = SDL_GetKeyboardState(NULL);

    bs_Vector2 forward       = { (bs_float)sin(*pRotation * (BS_PI / 180)),
              (bs_float)cos(*pRotation * (BS_PI / 180)) };
    bs_Vector2 velocity      = { 0, 0 };
    bs_float   rotationSpeed = .3f;

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

void UpdatePlayer(Player* pPlayer, double dt, SDL_Event* pEvent)
{
    double currentTime = SDL_GetTicks();

    dt = currentTime;
    pPlayer->playerForward = Update2DPlayer(&pPlayer->playerPos, &pPlayer->playerRot, dt);

    pPlayer->playerForward.x += (0.00001 * dt);
    pPlayer->playerForward.y += (0.00001 * dt);
}