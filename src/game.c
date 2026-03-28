#include "game.h"
#include "gfx.h"
#include <string.h>

char scrambled[50] = "ELPPA";    // dummy scrambled word
char input[50] = "";              // user input
int input_index = 0;
int show_correct = 0;             // flag to display "Correct!" message

// ----------------- DRAW GAME SCREEN -----------------
void draw_game() {
    gfx_color(0,0,0); // black text

    gfx_text("Unscramble this word:", 100, 100, 0);
    gfx_text(scrambled, 100, 150, 0);

    gfx_text("Your answer:", 100, 250, 0);
    gfx_text(input, 100, 300, 0);

    if (show_correct) {
        gfx_text("Correct!", 100, 400, 0);
    }

    gfx_text("Press [q] to return to menu", 100, 500, 0);
}

// ----------------- HANDLE INPUT -----------------
GameState game_handle_input(char c) {
    if (c == 'q') {
        // go back to menu
        input_index = 0;
        input[0] = '\0';
        show_correct = 0;
        return STATE_MENU;
    }

    if (c == 13) { // ENTER
        input[input_index] = '\0';
        if (strcmp(input, "APPLE") == 0) {
            show_correct = 1;
        } else {
            show_correct = 0;
        }
        input_index = 0;
        input[0] = '\0';
    }
    else if (c == 8) { // BACKSPACE
        if (input_index > 0) {
            input_index--;
            input[input_index] = '\0';
        }
    }
    else {
        // add letter to input
        input[input_index++] = c;
        input[input_index] = '\0';
    }

    return STATE_PLAYING;
}
