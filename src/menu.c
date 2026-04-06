#include "menu.h"
#include "gfx.h"

void draw_menu() 
{
    int w = gfx_xsize();
    int h = gfx_ysize();

    gfx_color(0, 0, 0);

    gfx_text("WORD SCRAMBLE GAME", w/2 - 150, 100, 2);

    int btn_w = 300;
    int btn_h = 40;
    int x = (w - btn_w) / 2;

    int y1 = 180;
    int y2 = 240;
    int y3 = 300;

    // Buttons
    gfx_color(200, 200, 200);
    gfx_fillrectangle(x, y1, btn_w, btn_h);
    gfx_fillrectangle(x, y2, btn_w, btn_h);
    gfx_fillrectangle(x, y3, btn_w, btn_h);

    gfx_color(0, 0, 0);
    gfx_text("New Game", x + 100, y1 + 25, 1);
    gfx_text("Help", x + 120, y2 + 25, 1);
    gfx_text("Exit", x + 120, y3 + 25, 1);
}

GameState menu_handle_click(int mx, int my) 
{
    int w = gfx_xsize();
    int btn_w = 300;
    int btn_h = 40;
    int x = (w - btn_w) / 2;

    if (mx >= x && mx <= x + btn_w && my >= 180 && my <= 220)
    {
    	game_init();
		return STATE_PLAYING;
    }

    if (mx >= x && mx <= x + btn_w && my >= 240 && my <= 280)
    {
    	return STATE_HELP;
    }

    if (mx >= x && mx <= x + btn_w && my >= 300 && my <= 340)
    {
        return STATE_EXIT;
    }

    return STATE_MENU;
}

// -------- HELP --------
void draw_help() 
{
    int w = gfx_xsize();

    gfx_color(0, 0, 0);
    gfx_text("HELP", w/2 - 40, 150, 2);
    gfx_text("Use mouse to click buttons", w/2 - 150, 220, 1);
    gfx_text("Get highest score!", w/2 - 120, 250, 1);

    int btn_w = 300;
    int btn_h = 40;
    int x = (w - btn_w) / 2;
    int y = 320;

    gfx_color(200, 200, 200);
    gfx_fillrectangle(x, y, btn_w, btn_h);

    gfx_color(0, 0, 0);
    gfx_text("Back", x + 120, y + 25, 1);
}

GameState help_handle_click(int mx, int my) 
{
    int w = gfx_xsize();
    int btn_w = 300;
    int btn_h = 40;
    int x = (w - btn_w) / 2;
    int y = 320;

    if (mx >= x && mx <= x + btn_w && my >= y && my <= y + btn_h) 
    {
		return STATE_MENU;
    }

    return STATE_HELP;
}
