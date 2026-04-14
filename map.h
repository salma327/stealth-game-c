#ifndef MAP_H
#define MAP_H

#include <stdbool.h>

#include "raylib.h"

typedef enum TileType {
    TILE_FLOOR = 0,
    TILE_WALL = 1
} TileType;

typedef struct Map {
    int width;
    int height;
    int tile_size;
    unsigned char *tiles;
    Vector2 player_spawn;
    Vector2 exit_cell;
} Map;

bool map_init_from_strings(Map *map, int width, int height, int tile_size, const char *rows[]);
void map_unload(Map *map);
bool map_is_wall(const Map *map, int cell_x, int cell_y);
bool map_circle_blocked(const Map *map, Vector2 world_pos, float radius);
void map_draw(const Map *map, Vector2 offset);

#endif
