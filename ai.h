#ifndef AI_H
#define AI_H

#include <stdbool.h>

#include "enemy.h"
#include "player.h"

void ai_update_enemy(Enemy *enemy, const Player *player, const Map *map, float delta_time, float speed_scale);
void ai_update_all(EnemyManager *manager, const Player *player, const Map *map, float delta_time, float speed_scale);
bool ai_enemy_detects_player(const Enemy *enemy, const Player *player, const Map *map);
bool ai_any_enemy_detects(const EnemyManager *manager, const Player *player, const Map *map);

#endif
