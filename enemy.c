#include "enemy.h"

#include <stdlib.h>

bool enemy_manager_create(EnemyManager *manager, size_t count) {
    if (manager == NULL || count == 0) {
        return false;
    }

    manager->items = (Enemy *)calloc(count, sizeof(Enemy));
    if (manager->items == NULL) {
        manager->count = 0;
        return false;
    }

    manager->count = count;
    return true;
}

void enemy_manager_destroy(EnemyManager *manager) {
    if (manager == NULL) {
        return;
    }

    free(manager->items);
    manager->items = NULL;
    manager->count = 0;
}

void enemy_draw(const Enemy *enemy, Vector2 offset, bool draw_vision) {
    Color enemy_color;

    if (enemy == NULL) {
        return;
    }

    switch (enemy->type) {
        case ENEMY_PATROL:
            enemy_color = (Color){235, 90, 90, 255};
            break;
        case ENEMY_CHASER:
            enemy_color = (Color){255, 140, 70, 255};
            break;
        case ENEMY_GUARD:
            enemy_color = (Color){210, 60, 120, 255};
            break;
        default:
            enemy_color = RED;
            break;
    }

    if (draw_vision) {
        DrawCircleLines(
            (int)(offset.x + enemy->position.x),
            (int)(offset.y + enemy->position.y),
            enemy->vision_range,
            enemy->sees_player ? (Color){255, 80, 80, 220} : (Color){255, 170, 80, 120}
        );
    }

    DrawCircleV((Vector2){offset.x + enemy->position.x, offset.y + enemy->position.y}, enemy->radius, enemy_color);
}
