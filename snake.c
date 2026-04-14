#include "snake.h"

#include <stdlib.h>

static bool snake_is_opposite_direction(SnakeDirection a, SnakeDirection b) {
    return (a == SNAKE_DIR_UP && b == SNAKE_DIR_DOWN)
        || (a == SNAKE_DIR_DOWN && b == SNAKE_DIR_UP)
        || (a == SNAKE_DIR_LEFT && b == SNAKE_DIR_RIGHT)
        || (a == SNAKE_DIR_RIGHT && b == SNAKE_DIR_LEFT);
}

static GridPosition snake_direction_delta(SnakeDirection direction) {
    GridPosition delta = {0, 0};

    switch (direction) {
        case SNAKE_DIR_UP:
            delta.y = -1;
            break;
        case SNAKE_DIR_RIGHT:
            delta.x = 1;
            break;
        case SNAKE_DIR_DOWN:
            delta.y = 1;
            break;
        case SNAKE_DIR_LEFT:
            delta.x = -1;
            break;
        default:
            break;
    }

    return delta;
}

bool snake_init(
    Snake *snake,
    GridPosition start,
    size_t initial_length,
    size_t max_cells,
    SnakeDirection direction
) {
    if (snake == NULL || initial_length == 0 || max_cells < initial_length) {
        return false;
    }

    snake->segments = (GridPosition *)malloc(sizeof(GridPosition) * max_cells);
    if (snake->segments == NULL) {
        return false;
    }

    snake->capacity = max_cells;
    snake->length = 0;
    snake->grow_segments = 0;
    snake->direction = direction;
    snake->next_direction = direction;
    snake->previous_head = start;

    return snake_reset(snake, start, initial_length, direction);
}

void snake_deinit(Snake *snake) {
    if (snake == NULL) {
        return;
    }

    free(snake->segments);
    snake->segments = NULL;
    snake->length = 0;
    snake->capacity = 0;
    snake->grow_segments = 0;
}

bool snake_reset(
    Snake *snake,
    GridPosition start,
    size_t initial_length,
    SnakeDirection direction
) {
    size_t i;

    if (snake == NULL || snake->segments == NULL || initial_length == 0 || initial_length > snake->capacity) {
        return false;
    }

    snake->length = initial_length;
    snake->grow_segments = 0;
    snake->direction = direction;
    snake->next_direction = direction;
    snake->previous_head = start;

    for (i = 0; i < initial_length; ++i) {
        snake->segments[i] = start;
        switch (direction) {
            case SNAKE_DIR_UP:
                snake->segments[i].y += (int)i;
                break;
            case SNAKE_DIR_RIGHT:
                snake->segments[i].x -= (int)i;
                break;
            case SNAKE_DIR_DOWN:
                snake->segments[i].y -= (int)i;
                break;
            case SNAKE_DIR_LEFT:
                snake->segments[i].x += (int)i;
                break;
            default:
                break;
        }
    }

    return true;
}

void snake_set_direction(Snake *snake, SnakeDirection requested_direction) {
    if (snake == NULL) {
        return;
    }

    if (!snake_is_opposite_direction(snake->direction, requested_direction)) {
        snake->next_direction = requested_direction;
    }
}

GridPosition snake_get_next_head(const Snake *snake) {
    GridPosition delta;
    GridPosition next_head;

    if (snake == NULL || snake->length == 0) {
        GridPosition zero = {0, 0};
        return zero;
    }

    delta = snake_direction_delta(snake->next_direction);
    next_head = snake->segments[0];
    next_head.x += delta.x;
    next_head.y += delta.y;

    return next_head;
}

void snake_step(Snake *snake) {
    size_t i;
    GridPosition next_head;

    if (snake == NULL || snake->length == 0) {
        return;
    }

    snake->direction = snake->next_direction;
    snake->previous_head = snake->segments[0];
    next_head = snake_get_next_head(snake);

    if (snake->grow_segments > 0 && snake->length < snake->capacity) {
        snake->length += 1;
        snake->grow_segments -= 1;
        snake->segments[snake->length - 1] = snake->segments[snake->length - 2];
    }

    for (i = snake->length - 1; i > 0; --i) {
        snake->segments[i] = snake->segments[i - 1];
    }

    snake->segments[0] = next_head;
}

void snake_grow(Snake *snake, int segments_to_add) {
    if (snake == NULL || segments_to_add <= 0) {
        return;
    }

    snake->grow_segments += segments_to_add;
}

bool snake_contains_position(const Snake *snake, GridPosition position, bool include_head) {
    size_t i = include_head ? 0 : 1;

    if (snake == NULL || snake->length == 0) {
        return false;
    }

    for (; i < snake->length; ++i) {
        if (utils_positions_equal(snake->segments[i], position)) {
            return true;
        }
    }

    return false;
}

bool snake_self_collision(const Snake *snake) {
    if (snake == NULL || snake->length < 4) {
        return false;
    }

    return snake_contains_position(snake, snake->segments[0], false);
}
