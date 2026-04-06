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
void game_init();
void draw_timer_only();

GameState game_handle_input(char c);
GameState game_update();

extern int score;
extern int lives;

#endif
