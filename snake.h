#ifndef SNAKE_H
#define SNAKE_H

#include <stdbool.h>
#include <stddef.h>

#include "utils.h"

typedef enum SnakeDirection {
    SNAKE_DIR_UP = 0,
    SNAKE_DIR_RIGHT,
    SNAKE_DIR_DOWN,
    SNAKE_DIR_LEFT
} SnakeDirection;

typedef struct Snake {
    GridPosition *segments;
    size_t length;
    size_t capacity;
    int grow_segments;
    SnakeDirection direction;
    SnakeDirection next_direction;
    GridPosition previous_head;
} Snake;

bool snake_init(
    Snake *snake,
    GridPosition start,
    size_t initial_length,
    size_t max_cells,
    SnakeDirection direction
);

void snake_deinit(Snake *snake);

bool snake_reset(
    Snake *snake,
    GridPosition start,
    size_t initial_length,
    SnakeDirection direction
);

void snake_set_direction(Snake *snake, SnakeDirection requested_direction);
GridPosition snake_get_next_head(const Snake *snake);
void snake_step(Snake *snake);
void snake_grow(Snake *snake, int segments_to_add);
bool snake_contains_position(const Snake *snake, GridPosition position, bool include_head);
bool snake_self_collision(const Snake *snake);

#endif
