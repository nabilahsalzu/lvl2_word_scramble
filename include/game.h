#ifndef GAME_H
#define GAME_H

#include "gfx.h"

typedef enum {
    STATE_MENU,
    STATE_HELP,
    STATE_PLAYING,
    STATE_EXIT
} GameState;

// -------- GAME FUNCTIONS --------
void draw_game();
GameState game_handle_input(char c);
GameState game_update();
void game_init();

#endif
