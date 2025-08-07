#include "../../include/game/game.h"
#include "../../include/engine/engine.h"
#include "../engine/engine.cpp"
#include "../renderer/renderer.cpp"


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
        }
        // if(IsKeyPressed(KEY_F5)) {
        // DEBUG_FLAG = !DEBUG_FLAG;
        // }

        UpdatePlayer(&player, delta_time, &event);

        Draw(&player, delta_time, GetWindowSettings());
    }


    Shutdown();
}
