#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

/* ── Allocation sécurisée ─────────────────────────────────────────────── */

/* Alloue 'size' octets ; quitte le programme en cas d'échec. */
void *safe_malloc(size_t size);

/* Alloue et met à zéro un tableau de 'count' éléments de 'size' octets. */
void *safe_calloc(size_t count, size_t size);

/* Libère *ptr et met *ptr à NULL pour éviter les doubles libérations. */
void  safe_free(void **ptr);

/* ── Mathématiques utilitaires ────────────────────────────────────────── */

/* Constante π portable (M_PI n'est pas garanti par la norme C99). */
#define GAME_PI 3.14159265358979323846f

/* Limite 'value' à l'intervalle [min, max]. */
float clampf(float value, float min, float max);

/* Convertit des degrés en radians. */
float deg2rad(float degrees);

/* Retourne la différence angulaire signée entre a et b, dans [-180, 180]. */
float angle_diff(float a, float b);

#endif /* UTILS_H */
