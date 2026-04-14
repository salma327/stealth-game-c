#ifndef LEVEL_H
#define LEVEL_H

#include <stdbool.h>

#include "enemy.h"
#include "map.h"

typedef struct Level {
    int index;
    float enemy_speed_scale;
    Map map;
    EnemyManager enemies;
} Level;

bool level_load(Level *level, int level_index, int tile_size);
void level_unload(Level *level);
int level_total_count(void);

#endif
