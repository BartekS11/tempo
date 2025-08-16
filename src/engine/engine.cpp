#include "../../include/engine/engine.h"
#include "../../include/g_lib.h"
#include "../../include/utils.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

void Init(void)
{
    BS_TRACE("Initializing engine...");

    if(!SDL_Init(SDL_INIT_VIDEO)) {
        BS_ERROR("Couldn't initialize SDL: %s", SDL_GetError());
        BS_ASSERT(false, "SDL initialization failed");
    }

    if(!SDL_CreateWindowAndRenderer(windowSettingsDefault.title,
       windowSettingsDefault.screenWidth, windowSettingsDefault.screenHeight, SDL_WINDOW_RESIZABLE,
       &windowSettingsDefault.pWindow, &windowSettingsDefault.pRenderer)) {
        BS_ERROR("Couldn't create window and renderer: %s", SDL_GetError());

        BS_ASSERT(false, "SDL window and renderer creation failed");
    }
}

// void UpdatePlayerPosition2d() {

// };

// void Update(void)
// {
// }

void Shutdown(void)
{
    BS_TRACE("Shutting down engine...");

    SDL_DestroyTexture(windowSettingsDefault.pTexture);
    SDL_DestroyRenderer(windowSettingsDefault.pRenderer);
    SDL_DestroyWindow(windowSettingsDefault.pWindow);

    SDL_Quit();
}

WindowSettings* GetWindowSettings(void)
{
    bs_int32 screenWidth  = windowSettingsDefault.screenWidth;
    bs_int32 screenHeight = windowSettingsDefault.screenHeight;

    BS_ASSERT((windowSettingsDefault.screenWidth > 0 && windowSettingsDefault.screenHeight > 0),
    "Invalid screen dimensions: %d x %d", windowSettingsDefault.screenWidth,
    windowSettingsDefault.screenHeight);

    SDL_GetWindowSize(windowSettingsDefault.pWindow, &screenWidth, &screenHeight);
    return &windowSettingsDefault;
}

void SetWindowSettings(const char* title, bs_int screenWidth, bs_int screenHeight)
{
    BS_ASSERT((screenWidth > 0 && screenHeight > 0),
    "Invalid screen dimensions: %d x %d", screenWidth, screenHeight);

    windowSettingsDefault.title        = title;
    windowSettingsDefault.screenWidth  = screenWidth;
    windowSettingsDefault.screenHeight = screenHeight;

    SDL_SetWindowTitle(windowSettingsDefault.pWindow, title);
    SDL_SetWindowSize(windowSettingsDefault.pWindow, screenWidth, screenHeight);
}