#include <stdio.h>

#include "raylib.h"

#include "game.h"

int main(void) {
    const int screen_width = 1024;
    const int screen_height = 768;
    Game game;

    InitWindow(screen_width, screen_height, "Stealth Escape - Raylib C Project");
    SetTargetFPS(60);

    if (!game_init(&game, screen_width, screen_height)) {
        CloseWindow();
        fprintf(stderr, "Failed to initialize game.\n");
        return 1;
    }

    while (!WindowShouldClose()) {
        float delta_time = GetFrameTime();

        game_update(&game, delta_time);

        BeginDrawing();
        game_draw(&game);
        EndDrawing();
    }

    game_deinit(&game);
    CloseWindow();
    return 0;
}
