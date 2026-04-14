#include "ai.h"

#include <math.h>

#include "utils.h"

static Vector2 ai_normalize(Vector2 v) {
    const float len = sqrtf(v.x * v.x + v.y * v.y);

    if (len <= 0.0001f) {
        return (Vector2){0.0f, 0.0f};
    }

    return (Vector2){v.x / len, v.y / len};
}

static bool ai_line_of_sight(const Enemy *enemy, const Player *player, const Map *map) {
    const Vector2 delta = {
        player->position.x - enemy->position.x,
        player->position.y - enemy->position.y
    };
    const float distance = sqrtf(delta.x * delta.x + delta.y * delta.y);
    const int steps = (int)(distance / (float)(map->tile_size / 3)) + 1;
    int i;

    if (steps <= 1) {
        return true;
    }

    for (i = 1; i < steps; ++i) {
        const float t = (float)i / (float)steps;
        const Vector2 p = {
            enemy->position.x + delta.x * t,
            enemy->position.y + delta.y * t
        };
        const int cell_x = (int)(p.x / (float)map->tile_size);
        const int cell_y = (int)(p.y / (float)map->tile_size);

        if (map_is_wall(map, cell_x, cell_y)) {
            return false;
        }
    }

    return true;
}

bool ai_enemy_detects_player(const Enemy *enemy, const Player *player, const Map *map) {
    const float dist = utils_vec2_distance(enemy->position, player->position);

    if (dist > enemy->vision_range) {
        return false;
    }

    return ai_line_of_sight(enemy, player, map);
}

bool ai_any_enemy_detects(const EnemyManager *manager, const Player *player, const Map *map) {
    size_t i;

    if (manager == NULL) {
        return false;
    }

    for (i = 0; i < manager->count; ++i) {
        if (ai_enemy_detects_player(&manager->items[i], player, map)) {
            return true;
        }
    }

    return false;
}

static void ai_move_enemy(Enemy *enemy, const Map *map, float delta_time, Vector2 direction, float speed_scale) {
    const float speed = enemy->speed * speed_scale;
    const Vector2 normalized = ai_normalize(direction);
    Vector2 motion = {normalized.x * speed * delta_time, normalized.y * speed * delta_time};

    if (enemy->type == ENEMY_GUARD) {
        motion = (Vector2){0.0f, 0.0f};
    }

    {
        Vector2 next = enemy->position;
        next.x += motion.x;
        if (!map_circle_blocked(map, next, enemy->radius)) {
            enemy->position.x = next.x;
        }
    }

    {
        Vector2 next = enemy->position;
        next.y += motion.y;
        if (!map_circle_blocked(map, next, enemy->radius)) {
            enemy->position.y = next.y;
        }
    }

    enemy->velocity = motion;
}

static bool ai_try_move_enemy(Enemy *enemy, const Map *map, float delta_time, Vector2 direction, float speed_scale) {
    const Vector2 before = enemy->position;
    ai_move_enemy(enemy, map, delta_time, direction, speed_scale);
    return utils_vec2_distance(before, enemy->position) > 0.01f;
}

void ai_update_enemy(Enemy *enemy, const Player *player, const Map *map, float delta_time, float speed_scale) {
    Vector2 desired = {0.0f, 0.0f};

    if (enemy == NULL || player == NULL || map == NULL) {
        return;
    }

    enemy->sees_player = ai_enemy_detects_player(enemy, player, map);

    switch (enemy->type) {
        case ENEMY_PATROL: {
            const Vector2 target = enemy->patrol_points[enemy->patrol_target];
            const float dist_to_target = utils_vec2_distance(enemy->position, target);

            if (dist_to_target < 6.0f) {
                enemy->patrol_target = 1 - enemy->patrol_target;
            }

            desired = (Vector2){
                enemy->patrol_points[enemy->patrol_target].x - enemy->position.x,
                enemy->patrol_points[enemy->patrol_target].y - enemy->position.y
            };
            break;
        }
        case ENEMY_CHASER: {
            if (enemy->sees_player) {
                desired = (Vector2){
                    player->position.x - enemy->position.x,
                    player->position.y - enemy->position.y
                };
            } else {
                desired = (Vector2){
                    enemy->guard_anchor.x - enemy->position.x,
                    enemy->guard_anchor.y - enemy->position.y
                };
            }
            break;
        }
        case ENEMY_GUARD:
            desired = (Vector2){0.0f, 0.0f};
            break;
        default:
            break;
    }

    if (!ai_try_move_enemy(enemy, map, delta_time, desired, speed_scale)) {
        if (enemy->type == ENEMY_PATROL) {
            enemy->patrol_target = 1 - enemy->patrol_target;
            desired = (Vector2){
                enemy->patrol_points[enemy->patrol_target].x - enemy->position.x,
                enemy->patrol_points[enemy->patrol_target].y - enemy->position.y
            };
            ai_try_move_enemy(enemy, map, delta_time, desired, speed_scale);
        } else if (enemy->type == ENEMY_CHASER) {
            desired = (Vector2){
                enemy->guard_anchor.x - enemy->position.x,
                enemy->guard_anchor.y - enemy->position.y
            };
            ai_try_move_enemy(enemy, map, delta_time, desired, speed_scale);
        }
    }
}

void ai_update_all(EnemyManager *manager, const Player *player, const Map *map, float delta_time, float speed_scale) {
    size_t i;

    if (manager == NULL) {
        return;
    }

    for (i = 0; i < manager->count; ++i) {
        ai_update_enemy(&manager->items[i], player, map, delta_time, speed_scale);
    }
}
