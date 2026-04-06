#include "game.h"
#include "gfx.h"

void draw_game() {
    int w = gfx_xsize();
    int h = gfx_ysize();

    // Title
    gfx_color(0, 0, 0);
    gfx_text("GAME SCREEN", w/2 - 80, 100, 2);

    // Back button
    int btn_w = 300;
    int btn_h = 40;
    int btn_x = (w - btn_w) / 2;
    int btn_y = (h - btn_h) / 2;

    gfx_color(200, 200, 200);
    gfx_fillrectangle(btn_x, btn_y, btn_w, btn_h);

    gfx_color(0, 0, 0);
    gfx_text("Back", btn_x + 120, btn_y + 25, 1);
}

GameState game_handle_input(char event) {
    if (event == 1) {
        int w = gfx_xsize();
        int h = gfx_ysize();

        int btn_w = 300;
        int btn_h = 40;
        int btn_x = (w - btn_w) / 2;
        int btn_y = (h - btn_h) / 2;

        int mx = gfx_xpos();
        int my = gfx_ypos();

        if (mx >= btn_x && mx <= btn_x + btn_w &&
            my >= btn_y && my <= btn_y + btn_h) {
            return STATE_MENU;
        }
    }

    return STATE_PLAYING;
}
