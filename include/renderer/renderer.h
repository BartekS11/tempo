#pragma once

#include "../engine/engine.h"
#include "../game/game.h"
#include "../utils.h"

void Render3DMap(bs_Vector2 cameraPosition,
bs_float                    cameraRotation,
bs_int                      lineThickness,
bs_int                      fov,
WindowSettings*             pWindowSettings);

void RenderFlatMap(const bs_int map[MAP_WIDTH * MAP_HEIGHT]);

void Render2DPlayer(const bs_Vector2 position, bs_double dt);

void Draw(Player* player, bs_double dt, WindowSettings* pWindowSettings);