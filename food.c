#include "food.h"

#include "raylib.h"

bool food_spawn_random(Food *food, const Snake *snake, int grid_width, int grid_height) {
    int max_attempts;
    int attempt;
    GridPosition candidate;
    int y;
    int x;

    if (food == NULL || snake == NULL || grid_width <= 0 || grid_height <= 0) {
        return false;
    }

    if ((int)snake->length >= grid_width * grid_height) {
        return false;
    }

    max_attempts = grid_width * grid_height * 2;

    for (attempt = 0; attempt < max_attempts; ++attempt) {
        candidate.x = GetRandomValue(0, grid_width - 1);
        candidate.y = GetRandomValue(0, grid_height - 1);

        if (!snake_contains_position(snake, candidate, true)) {
            food->position = candidate;
            food->pulse_time = 0.0f;
            return true;
        }
    }

    for (y = 0; y < grid_height; ++y) {
        for (x = 0; x < grid_width; ++x) {
            candidate.x = x;
            candidate.y = y;
            if (!snake_contains_position(snake, candidate, true)) {
                food->position = candidate;
                food->pulse_time = 0.0f;
                return true;
            }
        }
    }

    return false;
}

void food_update(Food *food, float delta_time) {
    if (food == NULL) {
        return;
    }

    food->pulse_time += delta_time;
}
