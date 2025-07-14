#include "../../include/raylib.h"
#include "../engine/engine.cpp"
#include "../renderer/renderer.cpp"

static Player player{ .playerPos = { 1, 1 }, .playerRot = 0, .playerForward = { 0, 0 } };

void Run(void) {
    bs_double posX = 22, posY = 12;
    bs_double dirX = -1, dirY = 0;
    bs_double planeX = 0, planeY = 0.66;

    bs_double prev_dt = GetTime();

    Init();
    while(!WindowShouldClose()) {
        UpdatePlayer(&player, prev_dt);
        BeginDrawing();
        Draw(&player, prev_dt);

        EndDrawing();
    }


    Shutdown();
}
