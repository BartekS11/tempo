#pragma once

#include "../utils.h"
#include <SDL3/SDL.h>

typedef struct {
    SDL_Window*   pWindow;
    SDL_Renderer* pRenderer;
    SDL_Surface*  pSurface;
    SDL_Texture*  pTexture;
    const char*   title;
    SDL_Event     event;
    bs_int        screenWidth;
    bs_int        screenHeight;
} WindowSettings;

// BSNOTE: Move renderers, surface, texture and event to a separate struct
// which is not connected to the window settings.
static WindowSettings windowSettingsDefault = { .title = "DoomCaster DEV SDL",
    .screenWidth                                       = SCREENWIDTH,
    .screenHeight                                      = SCREENHEIGHT,
    .pWindow                                           = nullptr,
    .pRenderer                                         = nullptr,
    .pSurface                                          = nullptr,
    .pTexture                                          = nullptr,
    .event                                             = { 0 } };


void Init(void);

void Shutdown(void);

WindowSettings* GetWindowSettings(void);

void SetWindowSettings(const char* title, bs_int screenWidth, bs_int screenHeight);