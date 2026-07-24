# Phalsophobia

A terminal-based multiplayer ghost hunting game written in C inspired by Phasmophobia. Players explore procedurally configured house rooms, collect evidence (EMF, Spirit Box, Video Camera), manage sanity levels, and try to escape before the ghost catches them.

---

## Architecture & Project Structure

- **`main.c`**: Program entry point and main menu loop driver.
- **`gamelib.c`**: Core game logic, inventory system, map creation routines, and ghost interactions.
- **`gamelib.h`**: Header file containing data structures (`Giocatore`, `Zona_mappa`), enums, and function definitions.

---

## Technical Features

- **Circular Linked List**: Represents the house map zones, allowing smooth loop-based navigation across rooms.
- **Dynamic Memory Allocation**: Manages player instances, map nodes, and randomized turn orders dynamically.
- **Game Mechanics**:
  - **1 to 4 Players**: Local turn-based multiplayer.
  - **Difficulty Modes**: Amateur, Intermediate, and Nightmare modes affecting ghost encounters and sanity drops.
  - **Evidence & Sanity**: Collect key proof items and return them to the Caravan while managing your mental health to survive.

---

## How to Run

Compile the project with GCC:
   ```bash
   gcc -o phalsophobia main.c gamelib.c

   Run the game:
   ./phalsophobia
