#include "game.h"
#include "gfx.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define MAX_WORDS 70
#define MAX_LENGTH 20

// -------- GAME DATA --------
char words[MAX_WORDS][MAX_LENGTH];
int word_count = 0;

char current_word[MAX_LENGTH];
char scrambled[MAX_LENGTH];
char input[MAX_LENGTH] = "";

int input_index = 0;
int score = 0;
int lives = 3;

time_t start_time;
int time_limit = 10;

int current_index = 0;

// -------- LOAD WORDS --------
void load_words() {
    FILE *file = fopen("data/puzzle1.txt", "r");
    if (!file) return;

    while (word_count < MAX_WORDS &&
           fscanf(file, "%19s", words[word_count]) == 1) {
        word_count++;
    }

    fclose(file);
}

// -------- SCRAMBLE --------
void scramble_word(char word[]) {
    int len = strlen(word);
    char temp[MAX_LENGTH];
    strcpy(temp, word);

    do {
        for (int i = len - 1; i > 0; i--) {
            int j = rand() % (i + 1);
            char t = word[i];
            word[i] = word[j];
            word[j] = t;
        }
    } while (strcmp(word, temp) == 0);
}

// -------- START NEW ROUND --------
void next_word() {
    if (current_index >= word_count) return;

    strcpy(current_word, words[current_index]);
    strcpy(scrambled, current_word);
    scramble_word(scrambled);

    input_index = 0;
    input[0] = '\0';

    start_time = time(NULL);
    current_index++;
}

// -------- INIT GAME --------
void game_init() {
    srand(time(NULL));
    load_words();

    score = 0;
    lives = 3;
    current_index = 0;

    next_word();
}

// -------- DRAW GAME --------
void draw_game() {
    int w = gfx_xsize();
    int h = gfx_ysize();

    gfx_color(0,0,0);

    // Title
    gfx_text("WORD SCRAMBLE", w/2 - 120, 80, 2);

    // Lives + Score
    char info[100];
    sprintf(info, "Lives: %d    Score: %d", lives, score);
    gfx_text(info, w/2 - 120, 140, 1);

    // Timer
    int time_left = time_limit - (time(NULL) - start_time);
    char timer[50];
    sprintf(timer, "Time: %d", time_left);
    gfx_text(timer, w/2 - 50, 180, 1);

    // Word
    gfx_text("Scrambled:", w/2 - 100, 240, 1);
    gfx_text(scrambled, w/2 - 60, 280, 2);

    // Input
    gfx_text("Your guess:", w/2 - 100, 340, 1);
    gfx_text(input, w/2 - 60, 380, 2);

    // Back button
    gfx_color(200,200,200);
    gfx_fillrectangle(w/2 - 150, h - 100, 300, 40);

    gfx_color(0,0,0);
    gfx_text("Back", w/2 - 30, h - 80, 1);
}

// -------- HANDLE INPUT --------
GameState game_handle_input(char c) {

    int w = gfx_xsize();
    int h = gfx_ysize();

    // Mouse click
    if (c == 1) {
        int mx = gfx_xpos();
        int my = gfx_ypos();

        if (mx >= w/2 - 150 && mx <= w/2 + 150 &&
            my >= h - 100 && my <= h - 60) {
            return STATE_MENU;
        }
    }

    // ENTER
    else if (c == 13) {
        input[input_index] = '\0';

        if (strcmp(input, current_word) == 0) {
            score += 10;
        } else {
            lives--;
        }

        next_word();
    }

    // BACKSPACE
    else if (c == 8) {
        if (input_index > 0) {
            input_index--;
            input[input_index] = '\0';
        }
    }

    // NORMAL CHAR
    else if (input_index < MAX_LENGTH - 1) {
        input[input_index++] = c;
        input[input_index] = '\0';
    }

    return STATE_PLAYING;
}

// -------- UPDATE (TIMER) --------
GameState game_update() {
    int time_left = time_limit - (time(NULL) - start_time);

    if (time_left <= 0) {
        lives--;
        next_word();
    }

    if (lives <= 0) {
        return STATE_MENU;
    }

    return STATE_PLAYING;
}
