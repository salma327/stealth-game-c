#ifndef UTILS_H
#define UTILS_H

#include "raylib.h"

int utils_clampi(int value, int min, int max);
float utils_clampf(float value, float min, float max);
float utils_vec2_distance(Vector2 a, Vector2 b);
Vector2 utils_cell_to_world_center(int cell_x, int cell_y, int tile_size);

#endif
