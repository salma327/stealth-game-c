#include "level.h"

#include <string.h>

#include "utils.h"

#define LEVEL_WIDTH 20
#define LEVEL_HEIGHT 14

typedef struct EnemySpawn {
    EnemyType type;
    int x;
    int y;
    int patrol_a_x;
    int patrol_a_y;
    int patrol_b_x;
    int patrol_b_y;
    float speed;
    float vision;
} EnemySpawn;

static const char *LEVEL1_MAP[LEVEL_HEIGHT] = {
    "####################",
    "#P....#...........E#",
    "#.###.#.##########.#",
    "#...#.#..........#.#",
    "###.#.######.###.#.#",
    "#...#......#...#...#",
    "#.########.###.###.#",
    "#........#.....#...#",
    "#.######.#####.#.###",
    "#.#....#.....#.#...#",
    "#.#.##.#####.#.###.#",
    "#...##.......#.....#",
    "#.###############..#",
    "####################"
};

static const char *LEVEL2_MAP[LEVEL_HEIGHT] = {
    "####################",
    "#P....#........#..E#",
    "#..####..#####..#..#",
    "#..#........#..##..#",
    "#..#.######.#......#",
    "#..#.#....#.#.####.#",
    "#....#....#.#....#.#",
    "####.#.####.###..#.#",
    "#....#......#....#.#",
    "#.#########.#.####.#",
    "#.....#.....#......#",
    "#.###.#.##########.#",
    "#.....#............#",
    "####################"
};

static const char *LEVEL3_MAP[LEVEL_HEIGHT] = {
    "####################",
    "#P..#.....#.......E#",
    "##.#.#.##.#.##.#.###",
    "#..#.#..#.#..#.#...#",
    "#.##.##.#.##.#.###.#",
    "#....#..#....#...#.#",
    "#.####.######.###.##",
    "#.#....#....#....#.#",
    "#.#.####.##.######.#",
    "#.#......##.#......#",
    "#.#.#######.#..##.##",
    "#...#.....#.#...#..#",
    "###.#.###.#.###.##.#",
    "####################"
};

static const EnemySpawn LEVEL1_ENEMIES[] = {
    {ENEMY_PATROL, 8, 1, 8, 1, 17, 1, 78.0f, 96.0f}
};

static const EnemySpawn LEVEL2_ENEMIES[] = {
    {ENEMY_PATROL, 6, 8, 6, 8, 11, 8, 88.0f, 108.0f},
    {ENEMY_GUARD, 14, 4, 14, 4, 14, 4, 0.0f, 122.0f}
};

static const EnemySpawn LEVEL3_ENEMIES[] = {
    {ENEMY_PATROL, 6, 5, 6, 5, 10, 5, 78.0f, 84.0f},
    {ENEMY_GUARD, 17, 11, 17, 11, 17, 11, 0.0f, 68.0f},
    {ENEMY_CHASER, 14, 11, 14, 11, 14, 11, 62.0f, 74.0f}
};

static Vector2 level_cell_center(int x, int y, int tile_size) {
    return utils_cell_to_world_center(x, y, tile_size);
}

int level_total_count(void) {
    return 3;
}

void level_unload(Level *level) {
    if (level == NULL) {
        return;
    }

    enemy_manager_destroy(&level->enemies);
    map_unload(&level->map);
    memset(level, 0, sizeof(*level));
}

static bool level_spawn_enemies(Level *level, const EnemySpawn *spawns, int spawn_count, int tile_size) {
    int i;

    if (!enemy_manager_create(&level->enemies, (size_t)spawn_count)) {
        return false;
    }

    for (i = 0; i < spawn_count; ++i) {
        Enemy *enemy = &level->enemies.items[i];

        enemy->type = spawns[i].type;
        enemy->position = level_cell_center(spawns[i].x, spawns[i].y, tile_size);
        enemy->guard_anchor = enemy->position;
        enemy->patrol_points[0] = level_cell_center(spawns[i].patrol_a_x, spawns[i].patrol_a_y, tile_size);
        enemy->patrol_points[1] = level_cell_center(spawns[i].patrol_b_x, spawns[i].patrol_b_y, tile_size);
        enemy->patrol_target = 1;
        enemy->radius = 9.0f;
        enemy->speed = spawns[i].speed;
        enemy->vision_range = spawns[i].vision;
        enemy->velocity = (Vector2){0.0f, 0.0f};
        enemy->sees_player = false;
    }

    return true;
}

bool level_load(Level *level, int level_index, int tile_size) {
    bool ok = false;

    if (level == NULL || level_index < 0 || level_index >= level_total_count()) {
        return false;
    }

    level_unload(level);

    level->index = level_index;
    level->enemy_speed_scale = 1.0f + (float)level_index * 0.12f;
    if (level_index == 2) {
        level->enemy_speed_scale = 0.92f;
    }

    switch (level_index) {
        case 0:
            ok = map_init_from_strings(&level->map, LEVEL_WIDTH, LEVEL_HEIGHT, tile_size, LEVEL1_MAP)
                && level_spawn_enemies(level, LEVEL1_ENEMIES, (int)(sizeof(LEVEL1_ENEMIES) / sizeof(LEVEL1_ENEMIES[0])), tile_size);
            break;
        case 1:
            ok = map_init_from_strings(&level->map, LEVEL_WIDTH, LEVEL_HEIGHT, tile_size, LEVEL2_MAP)
                && level_spawn_enemies(level, LEVEL2_ENEMIES, (int)(sizeof(LEVEL2_ENEMIES) / sizeof(LEVEL2_ENEMIES[0])), tile_size);
            break;
        case 2:
            ok = map_init_from_strings(&level->map, LEVEL_WIDTH, LEVEL_HEIGHT, tile_size, LEVEL3_MAP)
                && level_spawn_enemies(level, LEVEL3_ENEMIES, (int)(sizeof(LEVEL3_ENEMIES) / sizeof(LEVEL3_ENEMIES[0])), tile_size);
            break;
        default:
            ok = false;
            break;
    }

    if (!ok) {
        level_unload(level);
    }

    return ok;
}
