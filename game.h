#ifndef GAME_H
#define GAME_H

#include <stdbool.h>

#include "level.h"
#include "player.h"

typedef enum GameState {
    GAME_STATE_MENU = 0,
    GAME_STATE_PLAYING,
    GAME_STATE_PAUSED,
    GAME_STATE_DETECTED,
    GAME_STATE_LEVEL_CLEAR,
    GAME_STATE_VICTORY
} GameState;

typedef struct Game {
    int screen_width;
    int screen_height;
    int cell_size;

    Level level;
    Player player;
    GameState state;

    int current_level;
    int total_levels;
    float level_timer;
    float total_timer;
    float event_timer;

    bool running;
    bool show_debug_vision;
} Game;

bool game_init(Game *game, int screen_width, int screen_height);
void game_deinit(Game *game);
void game_update(Game *game, float delta_time);
void game_draw(const Game *game);

#endif
