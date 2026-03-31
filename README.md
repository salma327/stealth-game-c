# Stealth Game en C

Un jeu de furtivité (stealth) 2D en vue du dessus, développé en **C99** avec la bibliothèque **Raylib**.

Le joueur doit traverser deux niveaux gardés par des ennemis équipés d'un cône de vision, en évitant d'être détecté et en atteignant la sortie.

---

## Aperçu du jeu

| Élément | Description |
|---------|-------------|
| 🟦 Joueur | Contrôlé au clavier ; peut se déplacer normalement ou s'accroupir |
| 🟠 Ennemi (patrouille) | Patrouille entre deux points prédéfinis |
| 🟡 Ennemi (alerte) | A repéré quelque chose, augmente la détection |
| 🔴 Ennemi (poursuite) | Poursuit activement le joueur |
| 🟩 Sortie | Tuile verte clignotante — objectif du niveau |
| 🌑 Zone d'ombre | Sol sombre ; la détection y est réduite de 50 % |

La **barre de détection** (en bas à gauche) monte quand un ennemi vous voit.  
Atteignez **100 %** → partie perdue.  
Cachez-vous dans les zones d'ombre et accroupissez-vous pour ralentir la détection.

---

## Structure du projet

```
stealth-game-c/
├── main.c      — Point d'entrée, boucle principale
├── stealth.h   — Déclarations : structures, constantes, prototypes
├── stealth.c   — Logique complète du jeu (carte, joueur, IA, dessin)
├── utils.h     — Prototypes des fonctions utilitaires
├── utils.c     — Allocation sécurisée, fonctions mathématiques
├── Makefile    — Règles de compilation
└── README.md   — Cette documentation
```

---

## Prérequis

### Dépendances système (Ubuntu / Debian)

```bash
sudo apt-get install -y \
    build-essential \
    libasound2-dev libx11-dev libxrandr-dev libxi-dev \
    libgl1-mesa-dev libglu1-mesa-dev libxcursor-dev \
    libxinerama-dev libwayland-dev libxkbcommon-dev
```

### Compilation de Raylib (depuis les sources)

```bash
git clone --depth=1 --branch 5.0 https://github.com/raysan5/raylib.git /tmp/raylib
make -C /tmp/raylib/src PLATFORM=PLATFORM_DESKTOP
sudo make install -C /tmp/raylib/src
```

Raylib est alors installé dans `/usr/local/include` et `/usr/local/lib`.

---

## Compilation

### Avec le Makefile (recommandé)

```bash
# Version optimisée
make

# Version débogage (AddressSanitizer activé)
make debug

# Compiler et lancer directement
make run

# Nettoyer les fichiers générés
make clean
```

### Commande manuelle (comme spécifiée dans le projet)

```bash
gcc -Wall -Wextra -fsanitize=address \
    -I/usr/local/include -L/usr/local/lib \
    -o stealth_game main.c stealth.c utils.c \
    -lraylib -lm -lpthread -ldl -lrt -lX11
```

---

## Exécution

```bash
./stealth_game
```

---

## Contrôles

| Touche | Action |
|--------|--------|
| `W` / `↑` | Monter |
| `S` / `↓` | Descendre |
| `A` / `←` | Gauche |
| `D` / `→` | Droite |
| `Shift gauche` | S'accroupir (vitesse réduite, détection plus lente) |
| `P` / `Échap` | Pause / Reprendre |
| `Entrée` | Démarrer / Confirmer |
| `R` | Recommencer / Niveau suivant |
| `Q` | Quitter / Menu principal |

---

## Architecture du code

### `utils.h` / `utils.c`
Fonctions réutilisables indépendantes du jeu :
- `safe_malloc` / `safe_calloc` / `safe_free` — gestion mémoire sécurisée
- `clampf` — limite une valeur dans un intervalle
- `deg2rad` / `angle_diff` — utilitaires trigonométriques

### `stealth.h`
Déclare toutes les structures (`Player`, `Enemy`, `Map`, `Game`), les constantes et les prototypes de fonctions.

### `stealth.c`
Implémentation complète :
- `init_game()` — initialise l'état global du jeu
- `load_level()` — charge la carte et crée les ennemis (malloc)
- `update_game()` — machine d'états principale (menu → jeu → pause → fin)
- `move_player()` — déplacement avec détection de collision par axe
- `update_enemy()` — IA : patrouille, détection (cône + ligne de vue), poursuite
- `check_los()` — ray-march pour tester la ligne de vue
- `is_in_fov()` — distance + angle + ligne de vue
- `draw_game()` — dispatcher du rendu selon l'état
- `free_resources()` — libère tous les tableaux alloués dynamiquement

### `main.c`
Point d'entrée minimal : initialise Raylib, lance la boucle principale, libère les ressources.

---

## Gestion mémoire

Chaque appel à `safe_malloc` / `safe_calloc` possède son `safe_free` correspondant :

| Allocation | Libération |
|------------|-----------|
| `game->enemies` (tableau d'`Enemy`) | `free_enemies()` → `safe_free` |
| `enemy->patrol_pts` (points de patrouille) | `free_enemies()` → `safe_free` par ennemi |

La fonction `free_resources()` est appelée à la fermeture de la fenêtre et au rechargement d'un niveau.

