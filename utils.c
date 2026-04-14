#include "utils.h"

#include <math.h>

int utils_clampi(int value, int min, int max) {
    if (value < min) {
        return min;
    }
    if (value > max) {
        return max;
    }
    return value;
}

float utils_clampf(float value, float min, float max) {
    if (value < min) {
        return min;
    }
    if (value > max) {
        return max;
    }
    return value;
    return value;
}

float utils_vec2_distance(Vector2 a, Vector2 b) {
    const float dx = a.x - b.x;
    const float dy = a.y - b.y;
    return sqrtf(dx * dx + dy * dy);
}

Vector2 utils_cell_to_world_center(int cell_x, int cell_y, int tile_size) {
    const float half = (float)tile_size * 0.5f;
    return (Vector2){(float)cell_x * (float)tile_size + half, (float)cell_y * (float)tile_size + half};
}
