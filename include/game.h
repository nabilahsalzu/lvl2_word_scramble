#ifndef GAME_H
#define GAME_H

#include "gfx.h"

typedef enum {
    STATE_MENU,
    STATE_HELP,
    STATE_PLAYING,
    STATE_EXIT
} GameState;

void draw_game();
GameState game_handle_input(char c);

#endif
