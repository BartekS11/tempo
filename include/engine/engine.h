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
    SDL_Window*   window;
    SDL_Renderer* renderer;
    SDL_Surface*  surface;
    SDL_Texture*  texture;
    SDL_Event     event;
} WindowSettingsV2;


static WindowSettings windowSettingsDefault = { .title = "DoomCaster DEV",
    .screenWidth                                       = SCREENWIDTH,
    .screenHeight                                      = SCREENHEIGHT };


static WindowSettingsV2 windowSettingsDefaultV2 = { .title = "DoomCaster DEV SDL",
    .screenWidth                                       = SCREENWIDTH,
    .screenHeight                                      = SCREENHEIGHT,
    .window                                            = nullptr,
    .renderer                                          = nullptr,
    .surface                                           = nullptr,
    .texture                                           = nullptr,
    .event                                             = { 0 } };