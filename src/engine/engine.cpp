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

    if(!SDL_CreateWindowAndRenderer(
       "Hello SDL", SCREENWIDTH, SCREENHEIGHT, SDL_WINDOW_RESIZABLE, &windowSettingsDefaultV2.window, &windowSettingsDefaultV2.renderer)) {
        BS_ERROR("Couldn't create window and renderer: %s", SDL_GetError());
        BS_ASSERT(false, "SDL window and renderer creation failed");
    }
    // SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT);
    // InitWindow(windowSettingsDefault.screenWidth,
    // windowSettingsDefault.screenHeight, windowSettingsDefault.title);
    SDL_Init(SDL_INIT_VIDEO);
    // SetTargetFPS(TARGETFPS);
}

void UpdatePlayerPosition2d() {

};

void Update(void)
{
}

static void Shutdown(void)
{

    SDL_DestroyTexture(windowSettingsDefaultV2.texture);
    SDL_DestroyRenderer(windowSettingsDefaultV2.renderer);
    SDL_DestroyWindow(windowSettingsDefaultV2.window);

    SDL_Quit();
    // CloseWindow();
}

WindowSettings* GetWindowSettings(void)
{
    if(IsWindowResized()) {
        // BS NOTE: Update the window settings if the window has been resized
        windowSettingsDefault.screenWidth  = GetScreenWidth();
        windowSettingsDefault.screenHeight = GetScreenHeight();
    }

    BS_ASSERT((windowSettingsDefault.screenWidth > 0 && windowSettingsDefault.screenHeight > 0),
    "Invalid screen dimensions: %d x %d", windowSettingsDefault.screenWidth,
    windowSettingsDefault.screenHeight);

    return &windowSettingsDefault;
}