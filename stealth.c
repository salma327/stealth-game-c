#include "stealth.h"
#include "utils.h"
#include <math.h>
#include <string.h>

/* ─────────────────────────────────────────────────────────────────────────
 * Données des niveaux
 * 0 = sol  1 = mur  2 = sortie  3 = ombre
 * Taille : MAP_HEIGHT(18) lignes × MAP_WIDTH(25) colonnes
 * ───────────────────────────────────────────────────────────────────────── */

static const int level1_map[MAP_HEIGHT][MAP_WIDTH] = {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}, /* ligne  0 */
    {1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1}, /* ligne  1 */
    {1,0,1,1,1,1,1,0,1,1,1,0,1,0,1,1,1,0,1,1,1,1,1,0,1}, /* ligne  2 */
    {1,0,1,0,0,0,1,0,0,0,1,3,3,3,1,0,0,0,1,0,0,0,1,0,1}, /* ligne  3 */
    {1,0,1,0,1,0,1,1,1,0,1,1,1,1,1,0,1,1,1,0,1,0,1,0,1}, /* ligne  4 */
    {1,3,3,3,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,3,3,3,1}, /* ligne  5 */
    {1,0,1,0,1,0,1,1,1,0,1,1,1,1,1,0,1,1,1,0,1,0,1,0,1}, /* ligne  6 */
    {1,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,1}, /* ligne  7 */
    {1,0,1,1,1,0,1,0,1,1,1,0,1,0,1,1,1,0,1,0,1,1,1,0,1}, /* ligne  8 */
    {1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1}, /* ligne  9 */
    {1,0,1,1,1,0,1,0,1,1,1,0,1,0,1,1,1,0,1,0,1,1,1,0,1}, /* ligne 10 */
    {1,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,1}, /* ligne 11 */
    {1,0,1,0,1,0,1,1,1,0,1,1,1,1,1,0,1,1,1,0,1,0,1,0,1}, /* ligne 12 */
    {1,3,3,3,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,3,3,3,1}, /* ligne 13 */
    {1,0,1,0,1,0,1,1,1,0,1,1,1,1,1,0,1,1,1,0,1,0,1,0,1}, /* ligne 14 */
    {1,0,1,0,0,0,1,0,0,0,1,3,3,3,1,0,0,0,1,0,0,0,1,0,1}, /* ligne 15 */
    {1,0,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,0,1}, /* ligne 16 */
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,1}, /* ligne 17 */
};

static const int level2_map[MAP_HEIGHT][MAP_WIDTH] = {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}, /* ligne  0 */
    {1,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,1}, /* ligne  1 */
    {1,0,1,0,1,0,1,1,1,1,1,0,1,0,1,1,1,1,1,0,1,0,1,0,1}, /* ligne  2 */
    {1,0,1,0,0,0,1,3,3,3,1,0,0,0,1,3,3,3,1,0,0,0,1,0,1}, /* ligne  3 */
    {1,0,1,1,1,0,1,3,1,3,1,1,1,1,1,3,1,3,1,0,1,1,1,0,1}, /* ligne  4 */
    {1,0,0,0,0,0,0,3,0,3,0,0,0,0,0,3,0,3,0,0,0,0,0,0,1}, /* ligne  5 */
    {1,0,1,1,1,0,1,3,1,3,1,1,1,1,1,3,1,3,1,0,1,1,1,0,1}, /* ligne  6 */
    {1,0,1,0,0,0,1,3,3,3,1,0,0,0,1,3,3,3,1,0,0,0,1,0,1}, /* ligne  7 */
    {1,0,1,0,1,0,1,1,1,1,1,0,1,0,1,1,1,1,1,0,1,0,1,0,1}, /* ligne  8 */
    {1,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,1}, /* ligne  9 */
    {1,1,1,0,1,0,1,1,1,1,1,0,1,0,1,1,1,1,1,0,1,0,1,1,1}, /* ligne 10 */
    {1,0,0,0,0,0,1,3,3,3,1,0,0,0,1,3,3,3,1,0,0,0,0,0,1}, /* ligne 11 */
    {1,0,1,1,1,0,0,3,1,3,1,1,1,1,1,3,1,3,0,0,1,1,1,0,1}, /* ligne 12 */
    {1,0,1,0,0,0,1,3,1,3,0,0,0,0,0,3,1,3,1,0,0,0,1,0,1}, /* ligne 13 */
    {1,0,1,0,1,0,1,3,3,3,1,0,1,0,1,3,3,3,1,0,1,0,1,0,1}, /* ligne 14 */
    {1,0,0,0,1,0,1,1,1,1,1,0,1,0,1,1,1,1,1,0,1,0,0,0,1}, /* ligne 15 */
    {1,0,1,1,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,1,1,0,1}, /* ligne 16 */
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,1}, /* ligne 17 */
};

/* ─────────────────────────────────────────────────────────────────────────
 * Fonctions internes (non exportées)
 * ───────────────────────────────────────────────────────────────────────── */

/* Libère les ressources des ennemis (points de patrouille + tableau). */
static void free_enemies(Game *game)
{
    if (!game->enemies) return;
    for (int i = 0; i < game->enemy_count; i++) {
        safe_free((void **)&game->enemies[i].patrol_pts);
    }
    safe_free((void **)&game->enemies);
    game->enemy_count = 0;
}

/* Crée un ennemi et lui alloue ses points de patrouille. */
static void setup_enemy(Enemy *e,
                        float sx, float sy,
                        float angle,
                        float fov, float range, float speed,
                        int pt_count)
{
    e->pos         = (Vector2){ sx, sy };
    e->angle       = angle;
    e->fov         = fov;
    e->range       = range;
    e->speed       = speed;
    e->state       = ENEMY_PATROL;
    e->alert_timer = 0.0f;
    e->patrol_count = pt_count;
    e->current_pt   = 0;
    e->patrol_pts   = (Vector2 *)safe_malloc((size_t)pt_count * sizeof(Vector2));
}

/* ─────────────────────────────────────────────────────────────────────────
 * Cycle de vie du jeu
 * ───────────────────────────────────────────────────────────────────────── */

void init_game(Game *game)
{
    game->state        = GAME_MENU;
    game->level        = 1;
    game->time_elapsed = 0.0f;
    game->enemies      = NULL;
    game->enemy_count  = 0;
    memset(&game->map, 0, sizeof(game->map));
    memset(&game->player, 0, sizeof(game->player));
    load_level(game, 1);
}

void load_level(Game *game, int level)
{
    /* Libération des ressources du niveau précédent */
    free_enemies(game);

    game->level        = level;
    game->time_elapsed = 0.0f;

    /* Copie de la carte */
    const int (*src)[MAP_WIDTH] = (level == 2) ? level2_map : level1_map;
    game->map.width  = MAP_WIDTH;
    game->map.height = MAP_HEIGHT;
    memcpy(game->map.tiles, src, sizeof(game->map.tiles));

    /* Position initiale du joueur : tuile (1, 1) */
    float half = TILE_SIZE / 2.0f;
    game->player.pos       = (Vector2){ 1 * TILE_SIZE + half, 1 * TILE_SIZE + half };
    game->player.speed     = NORMAL_SPEED;
    game->player.crouching = false;
    game->player.detection = 0.0f;

    /* ── Création des ennemis ── */
    if (level == 1) {
        game->enemy_count = 2;
        game->enemies = (Enemy *)safe_calloc((size_t)game->enemy_count, sizeof(Enemy));

        /* Ennemi 0 : patrouille horizontale ligne 5, col 5 → col 19 */
        setup_enemy(&game->enemies[0],
                    5 * TILE_SIZE + half, 5 * TILE_SIZE + half,
                    0.0f, ENEMY_FOV, ENEMY_RANGE, ENEMY_SPEED, 2);
        game->enemies[0].patrol_pts[0] = (Vector2){ 5 * TILE_SIZE + half,  5 * TILE_SIZE + half };
        game->enemies[0].patrol_pts[1] = (Vector2){ 19 * TILE_SIZE + half, 5 * TILE_SIZE + half };

        /* Ennemi 1 : patrouille horizontale ligne 13, col 19 → col 5 */
        setup_enemy(&game->enemies[1],
                    19 * TILE_SIZE + half, 13 * TILE_SIZE + half,
                    180.0f, ENEMY_FOV, ENEMY_RANGE, ENEMY_SPEED, 2);
        game->enemies[1].patrol_pts[0] = (Vector2){ 19 * TILE_SIZE + half, 13 * TILE_SIZE + half };
        game->enemies[1].patrol_pts[1] = (Vector2){ 5 * TILE_SIZE + half,  13 * TILE_SIZE + half };

    } else { /* level == 2 */
        game->enemy_count = 4;
        game->enemies = (Enemy *)safe_calloc((size_t)game->enemy_count, sizeof(Enemy));

        /* Ennemi 0 : horizontal ligne 5, col 1 → col 23 */
        setup_enemy(&game->enemies[0],
                    1 * TILE_SIZE + half, 5 * TILE_SIZE + half,
                    0.0f, ENEMY_FOV, ENEMY_RANGE, ENEMY_SPEED, 2);
        game->enemies[0].patrol_pts[0] = (Vector2){ 1 * TILE_SIZE + half,  5 * TILE_SIZE + half };
        game->enemies[0].patrol_pts[1] = (Vector2){ 23 * TILE_SIZE + half, 5 * TILE_SIZE + half };

        /* Ennemi 1 : vertical colonne 5, ligne 1 → ligne 16 */
        setup_enemy(&game->enemies[1],
                    5 * TILE_SIZE + half, 1 * TILE_SIZE + half,
                    90.0f, ENEMY_FOV, ENEMY_RANGE, ENEMY_SPEED * 0.95f, 2);
        game->enemies[1].patrol_pts[0] = (Vector2){ 5 * TILE_SIZE + half, 1 * TILE_SIZE + half };
        game->enemies[1].patrol_pts[1] = (Vector2){ 5 * TILE_SIZE + half, 16 * TILE_SIZE + half };

        /* Ennemi 2 : vertical colonne 19, ligne 16 → ligne 1 (sens inverse) */
        setup_enemy(&game->enemies[2],
                    19 * TILE_SIZE + half, 16 * TILE_SIZE + half,
                    270.0f, ENEMY_FOV, ENEMY_RANGE, ENEMY_SPEED * 0.95f, 2);
        game->enemies[2].patrol_pts[0] = (Vector2){ 19 * TILE_SIZE + half, 16 * TILE_SIZE + half };
        game->enemies[2].patrol_pts[1] = (Vector2){ 19 * TILE_SIZE + half, 1 * TILE_SIZE + half };

        /* Ennemi 3 : horizontal ligne 17, col 1 → col 22 (garde la sortie) */
        setup_enemy(&game->enemies[3],
                    1 * TILE_SIZE + half, 17 * TILE_SIZE + half,
                    0.0f, ENEMY_FOV, ENEMY_RANGE * 1.1f, ENEMY_SPEED * 1.1f, 2);
        game->enemies[3].patrol_pts[0] = (Vector2){ 1 * TILE_SIZE + half,  17 * TILE_SIZE + half };
        game->enemies[3].patrol_pts[1] = (Vector2){ 22 * TILE_SIZE + half, 17 * TILE_SIZE + half };
    }
}

void free_resources(Game *game)
{
    free_enemies(game);
}

/* ─────────────────────────────────────────────────────────────────────────
 * Carte
 * ───────────────────────────────────────────────────────────────────────── */

bool is_wall(const Map *map, int tx, int ty)
{
    if (tx < 0 || tx >= map->width || ty < 0 || ty >= map->height)
        return true; /* hors limites = mur */
    return map->tiles[ty][tx] == TILE_WALL;
}

bool is_wall_at_pos(const Map *map, float px, float py)
{
    return is_wall(map, (int)(px / TILE_SIZE), (int)(py / TILE_SIZE));
}

TileType get_tile(const Map *map, float px, float py)
{
    int tx = (int)(px / TILE_SIZE);
    int ty = (int)(py / TILE_SIZE);
    if (tx < 0 || tx >= map->width || ty < 0 || ty >= map->height)
        return TILE_WALL;
    return (TileType)map->tiles[ty][tx];
}

/* ─────────────────────────────────────────────────────────────────────────
 * Ligne de vue (ray-march discrète)
 * ───────────────────────────────────────────────────────────────────────── */

bool check_los(const Map *map, Vector2 from, Vector2 to)
{
    float dx   = to.x - from.x;
    float dy   = to.y - from.y;
    float dist = sqrtf(dx * dx + dy * dy);
    if (dist < 1.0f) return true;

    /* Pas de ~8 px pour couvrir chaque tuile traversée */
    int steps = (int)(dist / 8.0f) + 1;
    for (int i = 1; i <= steps; i++) {
        float t  = (float)i / (float)(steps + 1);
        float px = from.x + dx * t;
        float py = from.y + dy * t;
        if (is_wall_at_pos(map, px, py)) return false;
    }
    return true;
}

/* ─────────────────────────────────────────────────────────────────────────
 * Champ de vision (cône)
 * ───────────────────────────────────────────────────────────────────────── */

bool is_in_fov(const Enemy *enemy, Vector2 target, const Map *map)
{
    float dx   = target.x - enemy->pos.x;
    float dy   = target.y - enemy->pos.y;
    float dist = sqrtf(dx * dx + dy * dy);

    /* 1. Distance */
    if (dist > enemy->range) return false;

    /* 2. Angle par rapport à la direction du regard */
    float angle_to = atan2f(dy, dx) * (180.0f / GAME_PI);
    if (angle_to < 0.0f) angle_to += 360.0f;
    if (fabsf(angle_diff(angle_to, enemy->angle)) > enemy->fov) return false;

    /* 3. Ligne de vue dégagée */
    return check_los(map, enemy->pos, target);
}

/* ─────────────────────────────────────────────────────────────────────────
 * Mouvement du joueur
 * ───────────────────────────────────────────────────────────────────────── */

void move_player(Player *player, const Map *map, float delta)
{
    player->crouching = IsKeyDown(KEY_LEFT_SHIFT);
    float spd = player->crouching ? CROUCH_SPEED : NORMAL_SPEED;

    float dx = 0.0f, dy = 0.0f;
    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))    dy -= spd * delta;
    if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))  dy += spd * delta;
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))  dx -= spd * delta;
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) dx += spd * delta;

    float r = PLAYER_RADIUS;

    /* Collision sur l'axe X (glissement possible sur Y) */
    if (dx != 0.0f) {
        float nx = player->pos.x + dx;
        if (!is_wall_at_pos(map, nx - r, player->pos.y) &&
            !is_wall_at_pos(map, nx + r, player->pos.y)) {
            player->pos.x = nx;
        }
    }

    /* Collision sur l'axe Y */
    if (dy != 0.0f) {
        float ny = player->pos.y + dy;
        if (!is_wall_at_pos(map, player->pos.x - r, ny) &&
            !is_wall_at_pos(map, player->pos.x + r, ny)) {
            player->pos.y = ny;
        }
    }
}

/* ─────────────────────────────────────────────────────────────────────────
 * IA ennemi
 * ───────────────────────────────────────────────────────────────────────── */

void update_enemy(Enemy *enemy, Player *player, const Map *map, float delta)
{
    /* ── Déplacement vers le prochain point de patrouille ── */
    Vector2 target = enemy->patrol_pts[enemy->current_pt];
    float   dx     = target.x - enemy->pos.x;
    float   dy     = target.y - enemy->pos.y;
    float   dist   = sqrtf(dx * dx + dy * dy);

    if (dist > 4.0f) {
        float spd = (enemy->state == ENEMY_CHASE)
                    ? enemy->speed * 1.5f
                    : enemy->speed;
        enemy->pos.x += (dx / dist) * spd * delta;
        enemy->pos.y += (dy / dist) * spd * delta;

        /* L'ennemi regarde dans sa direction de déplacement */
        float a = atan2f(dy, dx) * (180.0f / GAME_PI);
        if (a < 0.0f) a += 360.0f;
        enemy->angle = a;
    } else {
        /* Point atteint : passage au point suivant */
        enemy->current_pt = (enemy->current_pt + 1) % enemy->patrol_count;
    }

    /* ── Détection du joueur ── */
    bool sees = is_in_fov(enemy, player->pos, map);

    if (sees) {
        /* Facteur de détection selon la tuile et la posture */
        TileType tile = get_tile(map, player->pos.x, player->pos.y);
        float rate = DETECT_RATE;
        if (tile == TILE_SHADOW) rate *= 0.5f;
        if (player->crouching)   rate *= 0.5f;

        player->detection = clampf(player->detection + rate * delta, 0.0f, 1.0f);

        enemy->state       = (player->detection >= 0.6f) ? ENEMY_CHASE : ENEMY_ALERT;
        enemy->alert_timer = 2.5f; /* réinitialise le compte à rebours */

        /* En mode poursuite, le prochain waypoint est la position du joueur */
        if (enemy->state == ENEMY_CHASE) {
            enemy->patrol_pts[enemy->current_pt] = player->pos;
        }
    } else {
        /* Hors du cône : la détection décroît lentement */
        player->detection = clampf(player->detection - FADE_RATE * delta, 0.0f, 1.0f);

        if (enemy->state != ENEMY_PATROL) {
            enemy->alert_timer -= delta;
            if (enemy->alert_timer <= 0.0f) {
                enemy->state       = ENEMY_PATROL;
                enemy->alert_timer = 0.0f;
            }
        }
    }
}

/* ─────────────────────────────────────────────────────────────────────────
 * Mise à jour générale (machine d'états)
 * ───────────────────────────────────────────────────────────────────────── */

void update_game(Game *game)
{
    float delta = GetFrameTime();

    switch (game->state) {

        case GAME_MENU:
            if (IsKeyPressed(KEY_ENTER)) {
                load_level(game, 1);
                game->state = GAME_PLAYING;
            }
            if (IsKeyPressed(KEY_Q) || WindowShouldClose()) {
                game->state = GAME_QUIT;
            }
            break;

        case GAME_PLAYING:
            game->time_elapsed += delta;

            if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_P)) {
                game->state = GAME_PAUSED;
                break;
            }

            move_player(&game->player, &game->map, delta);

            for (int i = 0; i < game->enemy_count; i++) {
                update_enemy(&game->enemies[i], &game->player,
                             &game->map, delta);
            }

            /* Condition de défaite */
            if (game->player.detection >= 1.0f) {
                game->state = GAME_OVER;
                break;
            }

            /* Condition de victoire */
            if (get_tile(&game->map,
                         game->player.pos.x,
                         game->player.pos.y) == TILE_EXIT) {
                game->state = GAME_WIN;
            }
            break;

        case GAME_PAUSED:
            if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_P)) {
                game->state = GAME_PLAYING;
            }
            if (IsKeyPressed(KEY_Q)) {
                game->state = GAME_MENU;
            }
            break;

        case GAME_OVER:
            if (IsKeyPressed(KEY_R)) {
                load_level(game, game->level);
                game->state = GAME_PLAYING;
            }
            if (IsKeyPressed(KEY_Q) || IsKeyPressed(KEY_ESCAPE)) {
                game->state = GAME_MENU;
            }
            break;

        case GAME_WIN:
            if (IsKeyPressed(KEY_R)) {
                int next = (game->level < MAX_LEVELS) ? game->level + 1 : 1;
                load_level(game, next);
                game->state = GAME_PLAYING;
            }
            if (IsKeyPressed(KEY_Q) || IsKeyPressed(KEY_ESCAPE)) {
                game->state = GAME_MENU;
            }
            break;

        case GAME_QUIT:
            /* La boucle principale détecte cet état et arrête la boucle. */
            break;
    }
}

/* ─────────────────────────────────────────────────────────────────────────
 * Dessin
 * ───────────────────────────────────────────────────────────────────────── */

void draw_map(const Map *map)
{
    for (int y = 0; y < map->height; y++) {
        for (int x = 0; x < map->width; x++) {
            int px = x * TILE_SIZE;
            int py = y * TILE_SIZE;

            switch (map->tiles[y][x]) {
                case TILE_FLOOR:
                    DrawRectangle(px, py, TILE_SIZE, TILE_SIZE,
                                  (Color){55, 55, 55, 255});
                    break;

                case TILE_WALL:
                    DrawRectangle(px, py, TILE_SIZE, TILE_SIZE,
                                  (Color){28, 28, 32, 255});
                    /* arêtes lumineuses */
                    DrawRectangle(px, py, TILE_SIZE, 2,
                                  (Color){65, 65, 72, 255});
                    DrawRectangle(px, py, 2, TILE_SIZE,
                                  (Color){65, 65, 72, 255});
                    break;

                case TILE_EXIT: {
                    /* clignotement léger : variation basée sur le temps */
                    float t  = (float)GetTime();
                    int   bv = (int)clampf(180.0f + 75.0f * sinf(t * 4.0f), 0.0f, 255.0f);
                    DrawRectangle(px, py, TILE_SIZE, TILE_SIZE,
                                  (Color){0, (unsigned char)bv, 60, 255});
                    DrawRectangle(px + 6, py + 6, TILE_SIZE - 12, TILE_SIZE - 12,
                                  (Color){0, 255, 100, 200});
                    break;
                }

                case TILE_SHADOW:
                    DrawRectangle(px, py, TILE_SIZE, TILE_SIZE,
                                  (Color){20, 20, 45, 255});
                    break;

                default:
                    break;
            }
        }
    }
}

void draw_vision_cone(const Enemy *enemy)
{
    Color c;
    switch (enemy->state) {
        case ENEMY_PATROL: c = (Color){255, 200,  50,  35}; break;
        case ENEMY_ALERT:  c = (Color){255, 140,   0,  65}; break;
        case ENEMY_CHASE:  c = (Color){220,  30,  30, 100}; break;
        default:           c = (Color){255, 200,  50,  35}; break;
    }
    /* DrawCircleSector : angles exprimés en degrés, sens horaire depuis →  */
    DrawCircleSector(enemy->pos,
                     enemy->range,
                     enemy->angle - enemy->fov,
                     enemy->angle + enemy->fov,
                     20, c);
}

void draw_enemies(const Enemy *enemies, int count)
{
    for (int i = 0; i < count; i++) {
        const Enemy *e = &enemies[i];

        /* Cône de vision en premier (sous le sprite) */
        draw_vision_cone(e);

        /* Corps */
        Color body;
        switch (e->state) {
            case ENEMY_PATROL: body = ORANGE; break;
            case ENEMY_ALERT:  body = YELLOW; break;
            case ENEMY_CHASE:  body = RED;    break;
            default:           body = ORANGE; break;
        }
        DrawCircleV(e->pos, 11.0f, DARKGRAY); /* ombre */
        DrawCircleV(e->pos,  9.0f, body);

        /* Indicateur de direction (point avant) */
        float rad = deg2rad(e->angle);
        Vector2 tip = {
            e->pos.x + cosf(rad) * 15.0f,
            e->pos.y + sinf(rad) * 15.0f
        };
        DrawLineV(e->pos, tip, BLACK);
        DrawCircleV(tip, 3.0f, BLACK);
    }
}

void draw_player(const Player *player)
{
    /* Couleur : bleu clair debout, bleu foncé accroupi */
    Color body = player->crouching
                 ? (Color){30, 100, 200, 255}
                 : (Color){80, 180, 255, 255};

    float r = player->crouching ? PLAYER_RADIUS * 0.7f : PLAYER_RADIUS;

    DrawCircleV(player->pos, r + 2.0f, DARKGRAY); /* ombre */
    DrawCircleV(player->pos, r,         body);

    /* Indicateur d'état accroupi */
    if (player->crouching) {
        DrawCircleLines((int)player->pos.x, (int)player->pos.y,
                        r + 4.0f, (Color){30, 100, 200, 120});
    }
}

void draw_hud(const Game *game)
{
    const Player *p = &game->player;
    int bar_w = 200, bar_h = 18;
    int bar_x = 10, bar_y = SCREEN_HEIGHT - 30;

    /* ── Barre de détection ── */
    DrawRectangle(bar_x - 2, bar_y - 2, bar_w + 4, bar_h + 4, DARKGRAY);
    DrawRectangle(bar_x, bar_y, bar_w, bar_h, (Color){40, 40, 40, 255});

    int filled = (int)(p->detection * bar_w);
    /* Dégradé vert → jaune → rouge selon le niveau de détection */
    Color det_color;
    if      (p->detection < 0.5f) det_color = (Color){ 50, 200,  50, 255};
    else if (p->detection < 0.8f) det_color = (Color){230, 180,  30, 255};
    else                          det_color = (Color){220,  40,  40, 255};

    DrawRectangle(bar_x, bar_y, filled, bar_h, det_color);
    DrawRectangleLines(bar_x, bar_y, bar_w, bar_h, LIGHTGRAY);
    DrawText("DETECTION", bar_x, bar_y - 16, 12, LIGHTGRAY);

    /* ── Statut accroupi ── */
    if (p->crouching) {
        DrawText("[ACCROUPI]", bar_x + bar_w + 10, bar_y, 14,
                 (Color){80, 180, 255, 255});
    }

    /* ── Chronomètre ── */
    int mins = (int)(game->time_elapsed / 60.0f);
    int secs = (int)game->time_elapsed % 60;
    char timer_buf[32];
    snprintf(timer_buf, sizeof(timer_buf), "%02d:%02d", mins, secs);
    DrawText(timer_buf, SCREEN_WIDTH - 70, 8, 22, LIGHTGRAY);

    /* ── Numéro de niveau ── */
    char lvl_buf[32];
    snprintf(lvl_buf, sizeof(lvl_buf), "NIVEAU %d", game->level);
    DrawText(lvl_buf, 10, 8, 18, LIGHTGRAY);

    /* ── Rappel des contrôles ── */
    DrawText("WASD/Fleches:Deplacer  Shift:Accroupir  P/Esc:Pause",
             10, SCREEN_HEIGHT - 52, 11, (Color){130, 130, 130, 255});
}

void draw_menu(void)
{
    ClearBackground((Color){15, 15, 20, 255});

    /* Titre */
    DrawText("STEALTH", SCREEN_WIDTH / 2 - 120, 140, 72,
             (Color){80, 180, 255, 255});
    DrawText("GAME",    SCREEN_WIDTH / 2 - 60,  210, 72,
             (Color){80, 180, 255, 200});

    /* Sous-titre */
    DrawText("Evitez les gardes et atteignez la sortie",
             SCREEN_WIDTH / 2 - 195, 310, 20, LIGHTGRAY);

    /* Instructions */
    DrawText("ENTREE  - Nouvelle partie",
             SCREEN_WIDTH / 2 - 130, 380, 18, (Color){200, 200, 200, 255});
    DrawText("Q       - Quitter",
             SCREEN_WIDTH / 2 - 130, 408, 18, (Color){200, 200, 200, 255});

    /* Légende */
    DrawText("Zones sombres  = detection reduite",
             SCREEN_WIDTH / 2 - 160, 480, 14, (Color){120, 120, 180, 255});
    DrawText("Shift          = deplacement silencieux",
             SCREEN_WIDTH / 2 - 160, 498, 14, (Color){120, 120, 180, 255});

    /* Version */
    DrawText("v1.0", SCREEN_WIDTH - 45, SCREEN_HEIGHT - 22, 12,
             (Color){80, 80, 80, 255});
}

void draw_pause(void)
{
    /* Fond semi-transparent */
    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,
                  (Color){0, 0, 0, 140});

    DrawText("PAUSE",
             SCREEN_WIDTH / 2 - 70, SCREEN_HEIGHT / 2 - 60, 52,
             (Color){80, 180, 255, 255});
    DrawText("P / Echap  - Reprendre",
             SCREEN_WIDTH / 2 - 120, SCREEN_HEIGHT / 2 + 10, 20, LIGHTGRAY);
    DrawText("Q          - Menu principal",
             SCREEN_WIDTH / 2 - 120, SCREEN_HEIGHT / 2 + 36, 20, LIGHTGRAY);
}

void draw_game_over(void)
{
    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,
                  (Color){0, 0, 0, 160});

    DrawText("DETECTE !",
             SCREEN_WIDTH / 2 - 105, SCREEN_HEIGHT / 2 - 70, 52,
             (Color){220, 40, 40, 255});
    DrawText("Mission echouee",
             SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 10, 26, LIGHTGRAY);
    DrawText("R - Reessayer",
             SCREEN_WIDTH / 2 - 80, SCREEN_HEIGHT / 2 + 40, 20,
             (Color){200, 200, 200, 255});
    DrawText("Q - Menu principal",
             SCREEN_WIDTH / 2 - 80, SCREEN_HEIGHT / 2 + 66, 20,
             (Color){200, 200, 200, 255});
}

void draw_win(int level, float elapsed)
{
    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,
                  (Color){0, 0, 0, 150});

    DrawText("MISSION ACCOMPLIE !",
             SCREEN_WIDTH / 2 - 190, SCREEN_HEIGHT / 2 - 80, 42,
             (Color){0, 220, 100, 255});

    char buf[64];
    int mins = (int)(elapsed / 60.0f);
    int secs = (int)elapsed % 60;
    snprintf(buf, sizeof(buf), "Niveau %d termine en %02d:%02d", level, mins, secs);
    DrawText(buf, SCREEN_WIDTH / 2 - 180, SCREEN_HEIGHT / 2 - 20, 22, LIGHTGRAY);

    if (level < MAX_LEVELS) {
        DrawText("R - Niveau suivant",
                 SCREEN_WIDTH / 2 - 90, SCREEN_HEIGHT / 2 + 40, 20,
                 (Color){200, 200, 200, 255});
    } else {
        DrawText("R - Recommencer depuis le debut",
                 SCREEN_WIDTH / 2 - 155, SCREEN_HEIGHT / 2 + 40, 20,
                 (Color){200, 200, 200, 255});
    }
    DrawText("Q - Menu principal",
             SCREEN_WIDTH / 2 - 90, SCREEN_HEIGHT / 2 + 66, 20,
             (Color){200, 200, 200, 255});
}

/* ─────────────────────────────────────────────────────────────────────────
 * Point d'entrée du dessin (appelé depuis main.c)
 * ───────────────────────────────────────────────────────────────────────── */

void draw_game(const Game *game)
{
    BeginDrawing();
    ClearBackground((Color){10, 10, 15, 255});

    switch (game->state) {

        case GAME_MENU:
            draw_menu();
            break;

        case GAME_PLAYING:
            draw_map(&game->map);
            draw_enemies(game->enemies, game->enemy_count);
            draw_player(&game->player);
            draw_hud(game);
            break;

        case GAME_PAUSED:
            draw_map(&game->map);
            draw_enemies(game->enemies, game->enemy_count);
            draw_player(&game->player);
            draw_hud(game);
            draw_pause();
            break;

        case GAME_OVER:
            draw_map(&game->map);
            draw_enemies(game->enemies, game->enemy_count);
            draw_player(&game->player);
            draw_hud(game);
            draw_game_over();
            break;

        case GAME_WIN:
            draw_map(&game->map);
            draw_enemies(game->enemies, game->enemy_count);
            draw_player(&game->player);
            draw_hud(game);
            draw_win(game->level, game->time_elapsed);
            break;

        case GAME_QUIT:
            /* Aucun dessin nécessaire, la boucle principale va s'arrêter. */
            break;
    }

    EndDrawing();
}
