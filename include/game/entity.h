#pragma once

#include "../g_lib.h"
#include "../utils.h"

typedef struct {
    const char name[MAX_NAME_LENGHT];
    void (*cleanup)(void);
    void (*handle_events)(SDL_Event*);
    void (*update)(bs_float);
    void (*render)(SDL_Renderer*);
} Entity;

Entity entities[MAX_ENTITIES] = {}; // Array of entities
size_t entitiesCount          = 0;  // Number of entities in the array

// void RenderEntities(SDL_Renderer* pRenderer);

// void UpdateEntities(size_t entitiesCount, bs_float dt);
