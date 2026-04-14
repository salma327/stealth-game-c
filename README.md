# Stealth Escape Game in C (Raylib)

Stealth Escape is a desktop stealth game built in pure C (C99/C11 compatible) with Raylib. You control a hero trying to reach the green exit while avoiding enemy vision.

## Gameplay

- Move through walls and corridors without entering enemy detection radius.
- Enemy AI includes three personalities:
  - Patrol enemy: moves between two patrol points.
  - Guard enemy: stays in place and scans an area.
  - Chaser enemy: pursues when you get close or seen.
- If detected, the level restarts.
- Reaching the exit loads the next level.
- Complete all levels to win.

## Controls

- Move: W, A, S, D or Arrow keys
- Pause/Resume: P
- Restart current level: R
- Toggle vision circles: V
- Start/Continue from menus: Enter or Space

## File Structure

- main.c: Program entry and loop
- game.c / game.h: State machine, flow, and main rendering
- player.c / player.h: Hero input and movement with wall collision
- enemy.c / enemy.h: Enemy types, data, and drawing
- ai.c / ai.h: Enemy behavior and detection logic
- level.c / level.h: Multi-level maps and enemy spawns
- map.c / map.h: Grid map loading/collision/rendering
- utils.c / utils.h: Shared helper functions

## Build and Run

Linux example:

```bash
gcc -std=c11 -Wall -Wextra -pedantic main.c game.c player.c enemy.c level.c map.c ai.c utils.c -o stealth_game -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
./stealth_game
```

Windows MinGW example:

```bash
gcc -std=c11 -Wall -Wextra -pedantic main.c game.c player.c enemy.c level.c map.c ai.c utils.c -o stealth_game.exe -lraylib -lopengl32 -lgdi32 -lwinmm
stealth_game.exe
```

MSYS2 UCRT64 example:

```bash
gcc -std=c11 -Wall -Wextra -pedantic main.c game.c player.c enemy.c level.c map.c ai.c utils.c -o stealth_game.exe -lraylib -lopengl32 -lgdi32 -lwinmm
./stealth_game.exe
```

## Notes

- Levels increase in difficulty by enemy count and speed scale.
- Memory for map data and enemy arrays is allocated/freed per level load.
- Vision radius is shown for debugging and learning enemy behavior.
