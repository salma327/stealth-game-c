#ifndef STEALTH_H
#define STEALTH_H

#include <raylib.h>
#include <stdbool.h>

/* ── Constantes de configuration ─────────────────────────────────────── */

#define SCREEN_WIDTH   800
#define SCREEN_HEIGHT  576      /* 18 tuiles × 32 px                     */
#define TARGET_FPS     60

#define TILE_SIZE      32
#define MAP_WIDTH      25
#define MAP_HEIGHT     18
#define MAX_LEVELS     2

#define PLAYER_RADIUS  10.0f
#define NORMAL_SPEED   150.0f   /* pixels/sec en marche normale           */
#define CROUCH_SPEED    70.0f   /* pixels/sec accroupi                    */

#define DETECT_RATE     0.35f   /* augmentation de la détection / sec     */
#define FADE_RATE       0.12f   /* diminution de la détection / sec       */

#define ENEMY_SPEED     80.0f   /* vitesse de patrouille (pixels/sec)     */
#define ENEMY_FOV       40.0f   /* demi-angle du cône de vision (°)       */
#define ENEMY_RANGE    200.0f   /* portée de vision (pixels)              */

/* ── Types et structures ─────────────────────────────────────────────── */

typedef enum {
    TILE_FLOOR  = 0,  /* sol normal                                       */
    TILE_WALL   = 1,  /* mur infranchissable                              */
    TILE_EXIT   = 2,  /* sortie du niveau                                 */
    TILE_SHADOW = 3   /* zone d'ombre : détection réduite                 */
} TileType;

typedef enum {
    GAME_MENU,
    GAME_PLAYING,
    GAME_PAUSED,
    GAME_OVER,
    GAME_WIN,
    GAME_QUIT   /* état terminal : la boucle principale doit s'arrêter    */
} GameState;

typedef enum {
    ENEMY_PATROL,  /* patrouille normale                                  */
    ENEMY_ALERT,   /* a vu quelque chose, cherche                         */
    ENEMY_CHASE    /* pourchasse activement le joueur                     */
} EnemyState;

/* Données du personnage joueur. */
typedef struct {
    Vector2 pos;          /* position en pixels (centre du sprite)        */
    float   speed;        /* vitesse courante (pixels/sec)                */
    bool    crouching;    /* vrai si la touche Shift est maintenue        */
    float   detection;    /* niveau de détection [0.0 = libre, 1.0 = pris] */
} Player;

/* Données d'un ennemi. */
typedef struct {
    Vector2    pos;           /* position en pixels (centre)              */
    float      angle;         /* direction du regard en degrés (0 = →)   */
    float      fov;           /* demi-angle du cône de vision (°)         */
    float      range;         /* portée de vision (pixels)                */
    float      speed;         /* vitesse de déplacement (pixels/sec)      */
    EnemyState state;         /* état courant (patrouille / alerte / ...)  */
    float      alert_timer;   /* temps restant en état alerte (sec)       */
    Vector2   *patrol_pts;    /* tableau de points de patrouille (malloc) */
    int        patrol_count;  /* nombre de points de patrouille           */
    int        current_pt;    /* indice du prochain point cible           */
} Enemy;

/* Carte du niveau. */
typedef struct {
    int tiles[MAP_HEIGHT][MAP_WIDTH];
    int width;
    int height;
} Map;

/* État global du jeu. */
typedef struct {
    Player    player;
    Enemy    *enemies;      /* tableau alloué dynamiquement (malloc)      */
    int       enemy_count;
    Map       map;
    GameState state;
    int       level;
    float     time_elapsed; /* temps écoulé depuis le début du niveau     */
} Game;

/* ── Déclarations de fonctions ───────────────────────────────────────── */

/* Cycle de vie */
void init_game(Game *game);
void load_level(Game *game, int level);
void update_game(Game *game);
void draw_game(const Game *game);
void free_resources(Game *game);

/* Joueur */
void move_player(Player *player, const Map *map, float delta);

/* Ennemis */
void update_enemy(Enemy *enemy, Player *player, const Map *map, float delta);
bool is_in_fov(const Enemy *enemy, Vector2 target, const Map *map);

/* Carte */
bool    is_wall(const Map *map, int tx, int ty);
bool    is_wall_at_pos(const Map *map, float px, float py);
TileType get_tile(const Map *map, float px, float py);

/* Optique */
bool check_los(const Map *map, Vector2 from, Vector2 to);

/* Dessin */
void draw_map(const Map *map);
void draw_player(const Player *player);
void draw_enemies(const Enemy *enemies, int count);
void draw_vision_cone(const Enemy *enemy);
void draw_hud(const Game *game);
void draw_menu(void);
void draw_pause(void);
void draw_game_over(void);
void draw_win(int level, float elapsed);

#endif /* STEALTH_H */
