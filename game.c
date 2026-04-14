#include "game.h"

#include <stdio.h>

#include "ai.h"
#include "raylib.h"
#include "utils.h"

static Vector2 game_map_offset(const Game *game) {
    const int map_pixel_w = game->level.map.width * game->level.map.tile_size;
    const int map_pixel_h = game->level.map.height * game->level.map.tile_size;
    const int left = (game->screen_width - map_pixel_w) / 2;
    const int top = (game->screen_height - map_pixel_h) / 2 + 40;
    return (Vector2){(float)left, (float)top};
}

static bool game_load_level(Game *game, int level_index) {
    Vector2 spawn;

    if (!level_load(&game->level, level_index, game->cell_size)) {
        return false;
    }

    spawn = utils_cell_to_world_center((int)game->level.map.player_spawn.x, (int)game->level.map.player_spawn.y, game->cell_size);
    player_init(&game->player, spawn);
    game->level_timer = 0.0f;
    game->event_timer = 0.0f;
    game->current_level = level_index;
    return true;
}

static bool game_restart_level(Game *game) {
    return game_load_level(game, game->current_level);
}

static bool game_advance_level(Game *game) {
    const int next_level = game->current_level + 1;

    if (next_level >= game->total_levels) {
        game->state = GAME_STATE_VICTORY;
        return true;
    }

    if (!game_load_level(game, next_level)) {
        game->state = GAME_STATE_VICTORY;
        return false;
    }

    game->state = GAME_STATE_PLAYING;
    return true;
}

static bool game_player_reached_exit(const Game *game) {
    const Vector2 exit_world = utils_cell_to_world_center(
        (int)game->level.map.exit_cell.x,
        (int)game->level.map.exit_cell.y,
        game->level.map.tile_size
    );
    return utils_vec2_distance(game->player.position, exit_world) <= (float)game->cell_size * 0.35f;
}

static void game_update_playing(Game *game, float delta_time) {
    player_update(&game->player, &game->level.map, delta_time);
    ai_update_all(
        &game->level.enemies,
        &game->player,
        &game->level.map,
        delta_time,
        game->level.enemy_speed_scale
    );

    game->level_timer += delta_time;
    game->total_timer += delta_time;

    if (ai_any_enemy_detects(&game->level.enemies, &game->player, &game->level.map)) {
        game->state = GAME_STATE_DETECTED;
        game->event_timer = 1.15f;
        return;
    }

    if (game_player_reached_exit(game)) {
        game->state = GAME_STATE_LEVEL_CLEAR;
        game->event_timer = 0.9f;
    }
}

bool game_init(Game *game, int screen_width, int screen_height) {
    if (game == NULL || screen_width < 800 || screen_height < 600) {
        return false;
    }

    game->screen_width = screen_width;
    game->screen_height = screen_height;
    game->cell_size = 32;
    game->total_levels = level_total_count();
    game->current_level = 0;
    game->level_timer = 0.0f;
    game->total_timer = 0.0f;
    game->event_timer = 0.0f;
    game->state = GAME_STATE_MENU;
    game->running = true;
    game->show_debug_vision = true;

    game->level.index = 0;
    game->level.enemy_speed_scale = 1.0f;
    game->level.map.width = 0;
    game->level.map.height = 0;
    game->level.map.tile_size = 0;
    game->level.map.tiles = NULL;
    game->level.enemies.items = NULL;
    game->level.enemies.count = 0;

    return game_load_level(game, 0);
}

void game_deinit(Game *game) {
    if (game == NULL) {
        return;
    }

    level_unload(&game->level);
}

void game_update(Game *game, float delta_time) {
    if (game == NULL || !game->running) {
        return;
    }

    if (IsKeyPressed(KEY_V)) {
        game->show_debug_vision = !game->show_debug_vision;
    }

    if (IsKeyPressed(KEY_R) && game->state != GAME_STATE_MENU) {
        game_restart_level(game);
        game->state = GAME_STATE_PLAYING;
    }

    if (game->state == GAME_STATE_MENU) {
        if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
            game->state = GAME_STATE_PLAYING;
        }
        return;
    }

    if (game->state == GAME_STATE_VICTORY) {
        if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
            game->total_timer = 0.0f;
            game_load_level(game, 0);
            game->state = GAME_STATE_PLAYING;
        }
        return;
    }

    if (IsKeyPressed(KEY_P) && (game->state == GAME_STATE_PLAYING || game->state == GAME_STATE_PAUSED)) {
        game->state = (game->state == GAME_STATE_PLAYING) ? GAME_STATE_PAUSED : GAME_STATE_PLAYING;
    }

    if (game->state == GAME_STATE_PAUSED) {
        return;
    }

    if (game->state == GAME_STATE_PLAYING) {
        game_update_playing(game, delta_time);
        return;
    }

    if (game->state == GAME_STATE_DETECTED) {
        game->event_timer -= delta_time;
        if (game->event_timer <= 0.0f) {
            game_restart_level(game);
            game->state = GAME_STATE_PLAYING;
        }
        return;
    }

    if (game->state == GAME_STATE_LEVEL_CLEAR) {
        game->event_timer -= delta_time;
        if (game->event_timer <= 0.0f) {
            game_advance_level(game);
        }
    }
}

void game_draw(const Game *game) {
    Vector2 offset;
    Vector2 exit_world;
    Vector2 exit_screen;
    size_t i;
    char line[128];

    if (game == NULL) {
        return;
    }

    offset = game_map_offset(game);
    exit_world = utils_cell_to_world_center((int)game->level.map.exit_cell.x, (int)game->level.map.exit_cell.y, game->cell_size);
    exit_screen = (Vector2){offset.x + exit_world.x, offset.y + exit_world.y};

    ClearBackground((Color){18, 22, 30, 255});
    DrawRectangle(0, 0, game->screen_width, 78, (Color){26, 33, 45, 255});
    DrawText("STEALTH ESCAPE", 24, 20, 34, RAYWHITE);
    snprintf(line, sizeof(line), "Level: %d / %d", game->current_level + 1, game->total_levels);
    DrawText(line, 24, 56, 20, (Color){200, 210, 220, 255});

    snprintf(line, sizeof(line), "Level Time: %.1fs", game->level_timer);
    DrawText(line, 240, 56, 20, (Color){200, 210, 220, 255});

    snprintf(line, sizeof(line), "Total Time: %.1fs", game->total_timer);
    DrawText(line, 430, 56, 20, (Color){200, 210, 220, 255});

    DrawText("Move: WASD/Arrows  Pause: P  Restart: R  Vision: V", 630, 56, 18, (Color){190, 200, 212, 255});

    map_draw(&game->level.map, offset);

    DrawRectangle(
        (int)(exit_screen.x - (float)game->cell_size * 0.3f),
        (int)(exit_screen.y - (float)game->cell_size * 0.3f),
        (int)((float)game->cell_size * 0.6f),
        (int)((float)game->cell_size * 0.6f),
        (Color){80, 225, 110, 255}
    );

    for (i = 0; i < game->level.enemies.count; ++i) {
        enemy_draw(&game->level.enemies.items[i], offset, game->show_debug_vision);
    }

    player_draw(&game->player, offset);

    if (game->state == GAME_STATE_MENU) {
        DrawRectangle(0, 0, game->screen_width, game->screen_height, (Color){0, 0, 0, 150});
        DrawText("Reach the green exit without being seen", 260, game->screen_height / 2 - 70, 32, RAYWHITE);
        DrawText("Enemies: Patrol, Guard, Chaser", 340, game->screen_height / 2 - 18, 28, (Color){255, 200, 150, 255});
        DrawText("Press ENTER or SPACE to start", 340, game->screen_height / 2 + 36, 28, RAYWHITE);
    }

    if (game->state == GAME_STATE_PAUSED) {
        DrawRectangle(0, 0, game->screen_width, game->screen_height, (Color){0, 0, 0, 130});
        DrawText("PAUSED", game->screen_width / 2 - 90, game->screen_height / 2 - 10, 56, RAYWHITE);
    }

    if (game->state == GAME_STATE_DETECTED) {
        DrawRectangle(0, 0, game->screen_width, game->screen_height, (Color){120, 0, 0, 90});
        DrawText("DETECTED!", game->screen_width / 2 - 130, game->screen_height / 2 - 20, 58, (Color){255, 90, 90, 255});
        DrawText("Restarting level...", game->screen_width / 2 - 110, game->screen_height / 2 + 38, 26, RAYWHITE);
    }

    if (game->state == GAME_STATE_LEVEL_CLEAR) {
        DrawRectangle(0, 0, game->screen_width, game->screen_height, (Color){0, 100, 0, 85});
        DrawText("Level Clear!", game->screen_width / 2 - 135, game->screen_height / 2 + 6, 54, (Color){130, 255, 130, 255});
    }

    if (game->state == GAME_STATE_VICTORY) {
        DrawRectangle(0, 0, game->screen_width, game->screen_height, (Color){0, 0, 0, 165});
        DrawText("ESCAPE COMPLETE", game->screen_width / 2 - 220, game->screen_height / 2 - 24, 56, (Color){130, 255, 140, 255});
        DrawText("Press ENTER to play again", game->screen_width / 2 - 165, game->screen_height / 2 + 42, 30, RAYWHITE);
    }
}
