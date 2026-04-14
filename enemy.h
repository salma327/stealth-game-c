#ifndef ENEMY_H
#define ENEMY_H

#include <stdbool.h>
#include <stddef.h>

#include "map.h"

typedef enum EnemyType {
    ENEMY_PATROL = 0,
    ENEMY_CHASER,
    ENEMY_GUARD
} EnemyType;

typedef struct Enemy {
    EnemyType type;
    Vector2 position;
    Vector2 velocity;
    float radius;
    float speed;
    float vision_range;
    Vector2 patrol_points[2];
    int patrol_target;
    Vector2 guard_anchor;
    bool sees_player;
} Enemy;

typedef struct EnemyManager {
    Enemy *items;
    size_t count;
} EnemyManager;

bool enemy_manager_create(EnemyManager *manager, size_t count);
void enemy_manager_destroy(EnemyManager *manager);
void enemy_draw(const Enemy *enemy, Vector2 offset, bool draw_vision);

#endif
