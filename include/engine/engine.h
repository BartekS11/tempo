#pragma once
#include "../utils.h"
#include <SDL3/SDL.h>

typedef struct {
    const char* title;
    bs_int      screenWidth;
    bs_int      screenHeight;
} WindowSettings;

typedef struct {
    const char*   title;
    bs_int        screenWidth;
    bs_int        screenHeight;
    SDL_Window*   pWindow;
    SDL_Renderer* pRenderer;
    SDL_Surface*  pSurface;
    SDL_Texture*  pTexture;
    SDL_Event     event;
} WindowSettingsV2;

static WindowSettingsV2 windowSettingsDefaultV2 = { .title =
                                                    "DoomCaster DEV SDL",
    .screenWidth  = SCREENWIDTH,
    .screenHeight = SCREENHEIGHT,
    .pWindow      = nullptr,
    .pRenderer    = nullptr,
    .pSurface     = nullptr,
    .pTexture     = nullptr,
    .event        = { 0 } };