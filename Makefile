# ── Stealth Game — Makefile ──────────────────────────────────────────────
#
# Usage :
#   make          — compile le jeu (optimisé)
#   make debug    — compile avec AddressSanitizer et symboles de débogage
#   make clean    — supprime les fichiers générés
#   make run      — compile et lance le jeu
# ─────────────────────────────────────────────────────────────────────────

CC      = gcc
TARGET  = stealth_game
SRCS    = main.c stealth.c utils.c

# Raylib est installé dans /usr/local par défaut (compilation depuis les sources)
RAYLIB_INC ?= /usr/local/include
RAYLIB_LIB ?= /usr/local/lib

LIBS    = -lraylib -lm -lpthread -ldl -lrt -lX11
LDFLAGS = -L$(RAYLIB_LIB)

CFLAGS_COMMON  = -std=c99 -Wall -Wextra -I$(RAYLIB_INC)
CFLAGS_RELEASE = $(CFLAGS_COMMON) -O2
CFLAGS_DEBUG   = $(CFLAGS_COMMON) -g -fsanitize=address

# Règle par défaut : compilation optimisée
.PHONY: all debug clean run

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS_RELEASE) $(LDFLAGS) -o $@ $^ $(LIBS)

# Compilation avec débogage et AddressSanitizer
debug:
	$(CC) $(CFLAGS_DEBUG) $(LDFLAGS) -o $(TARGET)_debug $(SRCS) $(LIBS)

# Lancer le jeu directement
run: all
	./$(TARGET)

# Nettoyage
clean:
	rm -f $(TARGET) $(TARGET)_debug
