#ifndef MENU_H
#define MENU_H

#include "game.h"   // for GameState enum

// Draw the main menu
void draw_menu();

// Handle mouse click, return new state
GameState menu_handle_click(int mouse_x, int mouse_y);

// Help screen
void draw_help();

GameState help_handle_click(int mouse_x, int mouse_y);

#endif
