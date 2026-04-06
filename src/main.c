#include <stdio.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include "gfx.h"
#include "game.h"
#include "menu.h"

int main() 
{
    // Get screen size
    Display *d = XOpenDisplay(NULL);
    int screen = DefaultScreen(d);

    int screen_width = DisplayWidth(d, screen);
    int screen_height = DisplayHeight(d, screen);

    // 3/4 window size
    int win_width = screen_width * 3 / 4;
    int win_height = screen_height * 3 / 4;

    gfx_open(win_width, win_height, "WORD SCRAMBLE GAME");

    GameState state = STATE_MENU;
    int needs_redraw = 1;

    while (state != STATE_EXIT) {

        // -------- INPUT --------
        if (gfx_event_waiting()) {
            char event = gfx_wait();
            needs_redraw = 1;

            if (event == 1) { // mouse click
                int mx = gfx_xpos();
                int my = gfx_ypos();

                if (state == STATE_MENU) state = menu_handle_click(mx, my);
                else if (state == STATE_HELP) state = help_handle_click(mx, my);
                else if (state == STATE_PLAYING) state = game_handle_input(event);
            }
        }
        
        // -------- DRAW --------
        if (needs_redraw) {
            gfx_clear_color(255,255,255);
            gfx_clear();

            if(state == STATE_MENU) draw_menu();
            else if(state == STATE_HELP) draw_help();
            else if(state == STATE_PLAYING) draw_game();

            gfx_flush();
            needs_redraw = 0;
        }

        usleep(16000);
    }

    return 0;
}
