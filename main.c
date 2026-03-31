/*
 * main.c — Point d'entrée du Stealth Game
 *
 * Compilation :
 *   gcc -Wall -Wextra -fsanitize=address -o stealth_game \
 *       main.c stealth.c utils.c \
 *       -lraylib -lm -lpthread -ldl -lrt -lX11
 */

#include "stealth.h"
#include "utils.h"

int main(void)
{
    /* ── Initialisation de la fenêtre Raylib ── */
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Stealth Game");
    SetTargetFPS(TARGET_FPS);
    SetExitKey(KEY_NULL); /* gestion manuelle de la fermeture */

    /* ── Initialisation de l'état du jeu ── */
    Game game;
    init_game(&game);

    /* ── Boucle principale ── */
    while (!WindowShouldClose() && game.state != GAME_QUIT) {
        update_game(&game);
        if (game.state != GAME_QUIT) {
            draw_game(&game);
        }
    }

    /* ── Nettoyage ── */
    free_resources(&game);
    CloseWindow();

    return 0;
}
