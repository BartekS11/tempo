#include "../../include/raylib.h"
#include "../engine/engine.cpp"
#include "../renderer/renderer.cpp"

static Player player{ .playerPos = { 1, 1 }, .playerRot = 0, .playerForward = { 0, 0 } };

void Run(void)
{
    bs_double prev_dt = GetTime();

    Init();
    while(!WindowShouldClose()) {
        if(IsKeyPressed(KEY_F5)) {
            DEBUG_FLAG = !DEBUG_FLAG;
        }
        Update();
        UpdatePlayer(&player, prev_dt);
        BeginDrawing();
        Draw(&player, prev_dt, GetWindowSettings());

        EndDrawing();
    }


    Shutdown();
}
