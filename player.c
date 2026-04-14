#include "player.h"

#include <math.h>
#include <stddef.h>

static Vector2 player_get_input_vector(void) {
    Vector2 input = {0.0f, 0.0f};

    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) {
        input.y -= 1.0f;
    }
    if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) {
        input.y += 1.0f;
    }
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
        input.x -= 1.0f;
    }
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
        input.x += 1.0f;
    }

    return input;
}

void player_init(Player *player, Vector2 spawn_world) {
    if (player == NULL) {
        return;
    }

    player->position = spawn_world;
    player->radius = 9.0f;
    player->speed = 140.0f;
}

void player_update(Player *player, const Map *map, float delta_time) {
    Vector2 input;
    float length;
    Vector2 velocity;

    if (player == NULL || map == NULL) {
        return;
    }

    input = player_get_input_vector();
    length = sqrtf(input.x * input.x + input.y * input.y);

    if (length <= 0.0f) {
        return;
    }

    input.x /= length;
    input.y /= length;

    velocity = (Vector2){input.x * player->speed * delta_time, input.y * player->speed * delta_time};

    {
        Vector2 next = player->position;
        next.x += velocity.x;
        if (!map_circle_blocked(map, next, player->radius)) {
            player->position.x = next.x;
        }
    }

    {
        Vector2 next = player->position;
        next.y += velocity.y;
        if (!map_circle_blocked(map, next, player->radius)) {
            player->position.y = next.y;
        }
    }
}

void player_draw(const Player *player, Vector2 offset) {
    if (player == NULL) {
        return;
    }

    DrawCircleV((Vector2){offset.x + player->position.x, offset.y + player->position.y}, player->radius, (Color){70, 120, 255, 255});
}
