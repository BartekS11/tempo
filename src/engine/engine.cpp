#include "../../include/engine/engine.h"
#include "../../include/g_lib.h"
// #include "../../include/raylib.h"
#include "../../include/utils.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>


static void Init(void)
{

    if(!SDL_Init(SDL_INIT_VIDEO)) {
        BS_ERROR("Couldn't initialize SDL: %s", SDL_GetError());
        BS_ASSERT(false, "SDL initialization failed");
    }

    if(!SDL_CreateWindowAndRenderer(windowSettingsDefaultV2.title,
       windowSettingsDefaultV2.screenWidth, windowSettingsDefaultV2.screenHeight, SDL_WINDOW_RESIZABLE,
       &windowSettingsDefaultV2.pWindow, &windowSettingsDefaultV2.pRenderer)) {
        BS_ERROR("Couldn't create window and renderer: %s", SDL_GetError());

        BS_ASSERT(false, "SDL window and renderer creation failed");
    }
    // SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT);
    // InitWindow(windowSettingsDefault.screenWidth,
    // windowSettingsDefault.screenHeight, windowSettingsDefault.title);
    // SetTargetFPS(TARGETFPS);
}

void UpdatePlayerPosition2d() {

};

void Update(void)
{
}

static void Shutdown(void)
{

    SDL_DestroyTexture(windowSettingsDefaultV2.pTexture);
    SDL_DestroyRenderer(windowSettingsDefaultV2.pRenderer);
    SDL_DestroyWindow(windowSettingsDefaultV2.pWindow);

    SDL_Quit();
    // CloseWindow();
}

WindowSettingsV2* GetWindowSettings(void)
{
    bs_int32 screenWidth  = windowSettingsDefaultV2.screenWidth;
    bs_int32 screenHeight = windowSettingsDefaultV2.screenHeight;

    BS_ASSERT((windowSettingsDefaultV2.screenWidth > 0 &&
              windowSettingsDefaultV2.screenHeight > 0),
    "Invalid screen dimensions: %d x %d", windowSettingsDefaultV2.screenWidth,
    windowSettingsDefaultV2.screenHeight);

    SDL_GetWindowSize(windowSettingsDefaultV2.pWindow, &screenWidth, &screenHeight);
    return &windowSettingsDefaultV2;
}