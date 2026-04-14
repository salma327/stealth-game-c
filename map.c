#include "map.h"

#include <stdlib.h>
#include <string.h>

#include "utils.h"

bool map_init_from_strings(Map *map, int width, int height, int tile_size, const char *rows[]) {
    int y;
    int x;

    if (map == NULL || rows == NULL || width <= 0 || height <= 0 || tile_size <= 0) {
        return false;
    }

    map->width = width;
    map->height = height;
    map->tile_size = tile_size;
    map->tiles = (unsigned char *)malloc((size_t)width * (size_t)height);
    map->player_spawn = (Vector2){1.0f, 1.0f};
    map->exit_cell = (Vector2){(float)(width - 2), (float)(height - 2)};

    if (map->tiles == NULL) {
        return false;
    }

    for (y = 0; y < height; ++y) {
        if ((int)strlen(rows[y]) != width) {
            free(map->tiles);
            map->tiles = NULL;
            return false;
        }

        for (x = 0; x < width; ++x) {
            const char c = rows[y][x];
            unsigned char tile = TILE_FLOOR;

            if (c == '#') {
                tile = TILE_WALL;
            } else if (c == 'P') {
                map->player_spawn = (Vector2){(float)x, (float)y};
            } else if (c == 'E') {
                map->exit_cell = (Vector2){(float)x, (float)y};
            }

            map->tiles[y * width + x] = tile;
        }
    }

    return true;
}

void map_unload(Map *map) {
    if (map == NULL) {
        return;
    }

    free(map->tiles);
    map->tiles = NULL;
    map->width = 0;
    map->height = 0;
    map->tile_size = 0;
    map->player_spawn = (Vector2){0.0f, 0.0f};
    map->exit_cell = (Vector2){0.0f, 0.0f};
}

bool map_is_wall(const Map *map, int cell_x, int cell_y) {
    if (map == NULL || map->tiles == NULL) {
        return true;
    }

    if (cell_x < 0 || cell_y < 0 || cell_x >= map->width || cell_y >= map->height) {
        return true;
    }

    return map->tiles[cell_y * map->width + cell_x] == TILE_WALL;
}

bool map_circle_blocked(const Map *map, Vector2 world_pos, float radius) {
    const Vector2 samples[4] = {
        {world_pos.x - radius, world_pos.y},
        {world_pos.x + radius, world_pos.y},
        {world_pos.x, world_pos.y - radius},
        {world_pos.x, world_pos.y + radius}
    };
    int i;

    if (map == NULL) {
        return true;
    }

    for (i = 0; i < 4; ++i) {
        const int cell_x = (int)(samples[i].x / (float)map->tile_size);
        const int cell_y = (int)(samples[i].y / (float)map->tile_size);
        if (map_is_wall(map, cell_x, cell_y)) {
            return true;
        }
    }

    return false;
}

void map_draw(const Map *map, Vector2 offset) {
    const Color floor_color = (Color){26, 30, 37, 255};
    const Color wall_color = (Color){110, 114, 124, 255};
    int y;
    int x;

    if (map == NULL || map->tiles == NULL) {
        return;
    }

    for (y = 0; y < map->height; ++y) {
        for (x = 0; x < map->width; ++x) {
            const int tile = map->tiles[y * map->width + x];
            const int draw_x = (int)offset.x + x * map->tile_size;
            const int draw_y = (int)offset.y + y * map->tile_size;

            DrawRectangle(draw_x, draw_y, map->tile_size, map->tile_size, tile == TILE_WALL ? wall_color : floor_color);
        }
    }

    for (y = 0; y <= map->height; ++y) {
        DrawLine(
            (int)offset.x,
            (int)offset.y + y * map->tile_size,
            (int)offset.x + map->width * map->tile_size,
            (int)offset.y + y * map->tile_size,
            (Color){55, 60, 68, 80}
        );
    }

    for (x = 0; x <= map->width; ++x) {
        DrawLine(
            (int)offset.x + x * map->tile_size,
            (int)offset.y,
            (int)offset.x + x * map->tile_size,
            (int)offset.y + map->height * map->tile_size,
            (Color){55, 60, 68, 80}
        );
    }
}
