/*
1. draw menu
2. Return the new state based on input
*/

#include "menu.h"
#include "gfx.h"

// ----------------- MAIN MENU -----------------
void draw_menu() {
    gfx_color(0, 0, 0);

    // Title
    gfx_text("WORD SCRAMBLE GAMES", 280, 100, 2);

    // Draw buttons as rectangles
    gfx_color(200, 200, 200); // light gray
    gfx_fillrectangle(250, 180, 300, 40); // New Game
    gfx_fillrectangle(250, 240, 300, 40); // Help
    gfx_fillrectangle(250, 300, 300, 40); // Exit

    // Button text
    gfx_color(0, 0, 0); // black
    gfx_text("New Game", 370, 200, 1);
    gfx_text("Help", 370, 260, 1);
    gfx_text("Exit", 370, 320, 1);
}

// ----------------- HANDLE MOUSE CLICK -----------------
GameState menu_handle_click(int mouse_x, int mouse_y) {
    // New Game button
    if (mouse_x >= 250 && mouse_x <= 550 &&
        mouse_y >= 180 && mouse_y <= 220) {
        return STATE_PLAYING;
    }
    // Help button
    if (mouse_x >= 250 && mouse_x <= 550 &&
        mouse_y >= 240 && mouse_y <= 280) {
        return STATE_HELP;
    }
    // Exit button
    if (mouse_x >= 250 && mouse_x <= 550 &&
        mouse_y >= 300 && mouse_y <= 340) {
        return STATE_EXIT;
    }

    return STATE_MENU; // no button clicked
}

// ----------------- HELP SCREEN -----------------
void draw_help() {
    gfx_color(0, 0, 0);
    gfx_text("HELP", 370, 150, 2);
    gfx_text("Use mouse to click buttons", 250, 220, 1);
    gfx_text("Get highest score!", 250, 250, 1);

    // Back button
    gfx_color(200,200,200);
    gfx_fillrectangle(250, 300, 300, 40);
    gfx_color(0,0,0);
    gfx_text("Back", 370, 320, 1);
}

GameState help_handle_click(int mouse_x, int mouse_y) {
    // Back button
    if (mouse_x >= 250 && mouse_x <= 550 &&
        mouse_y >= 300 && mouse_y <= 340) {
        return STATE_MENU;
    }
    return STATE_HELP;
}
