#ifndef PLAYER_H
#define PLAYER_H

#include "map.h"

typedef struct Player {
    Vector2 position;
    float radius;
    float speed;
} Player;

void player_init(Player *player, Vector2 spawn_world);
void player_update(Player *player, const Map *map, float delta_time);
void player_draw(const Player *player, Vector2 offset);

#endif
