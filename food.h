#ifndef FOOD_H
#define FOOD_H

#include <stdbool.h>

#include "snake.h"
#include "utils.h"

typedef struct Food {
    GridPosition position;
    float pulse_time;
} Food;

bool food_spawn_random(Food *food, const Snake *snake, int grid_width, int grid_height);
void food_update(Food *food, float delta_time);

#endif
