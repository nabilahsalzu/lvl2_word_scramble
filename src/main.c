#include <stdio.h>
#include <unistd.h>
#include "gfx.h"
#include "game.h"
#include "menu.h"

int main() 
{
    gfx_open(800, 600, "WORD SCRAMBLE GAME");

    GameState state = STATE_MENU;
    int needs_redraw = 1;

    while (state != STATE_EXIT) {

        // 1. Handle input
        if (gfx_event_waiting()) {
            char event = gfx_wait();
            needs_redraw = 1;

            if (event == 1) { // mouse click
                int mx = gfx_xpos();
                int my = gfx_ypos();

                if (state == STATE_MENU) state = menu_handle_click(mx, my);
                else if (state == STATE_HELP) state = help_handle_click(mx, my);
            }
        }

        // 2. Draw only if needed
        if (needs_redraw) {
            gfx_clear_color(255,255,255);
            gfx_clear();

            if(state == STATE_MENU) draw_menu();
            else if(state == STATE_HELP) draw_help();
            else if(state == STATE_PLAYING) draw_game();

            gfx_flush();
            needs_redraw = 0; // reset flag
        }

        // 3. Sleep a bit to reduce CPU
        usleep(16000);  // ~60 FPS
    }

    return 0;
}
