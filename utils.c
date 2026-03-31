#include "utils.h"

/* ── Allocation sécurisée ─────────────────────────────────────────────── */

void *safe_malloc(size_t size)
{
    void *ptr = malloc(size);
    if (!ptr) {
        fprintf(stderr, "Erreur: malloc(%zu) a échoué\n", size);
        exit(EXIT_FAILURE);
    }
    return ptr;
}

void *safe_calloc(size_t count, size_t size)
{
    void *ptr = calloc(count, size);
    if (!ptr) {
        fprintf(stderr, "Erreur: calloc(%zu, %zu) a échoué\n", count, size);
        exit(EXIT_FAILURE);
    }
    return ptr;
}

void safe_free(void **ptr)
{
    if (ptr && *ptr) {
        free(*ptr);
        *ptr = NULL;
    }
}

/* ── Mathématiques utilitaires ────────────────────────────────────────── */

float clampf(float value, float min, float max)
{
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

float deg2rad(float degrees)
{
    return degrees * (GAME_PI / 180.0f);
}

float angle_diff(float a, float b)
{
    float diff = fmodf(a - b + 360.0f, 360.0f);
    if (diff > 180.0f) diff -= 360.0f;
    return diff;
}
