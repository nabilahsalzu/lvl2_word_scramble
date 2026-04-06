#include "game.h"
#include "gfx.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define MAX_WORDS 70
#define MAX_LENGTH 20
#define BUTTON_SIZE 40
#define BUTTON_SPACING 10

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

// Feedback System
char feedback_msg[100] = "";
time_t feedback_expiry = 0;
int showing_feedback = 0;

// For letter buttons
typedef struct {
    char letter;
    int x, y, width, height;
    int clicked;
} LetterButton;

LetterButton letter_buttons[MAX_LENGTH];
int button_count = 0;

// Buttons: Submit, Backspace, Reset, Quit
typedef struct {
    int x, y, w, h;
} Button;

Button submit_btn, back_btn, reset_btn, quit_btn;

// -------- LOAD WORDS --------
void load_words() {
    FILE *file = fopen("data/puzzle1.txt", "r");
    if (!file) return;

    word_count = 0;
    while (word_count < MAX_WORDS &&
           fscanf(file, "%19s", words[word_count]) == 1) {
        word_count++;
    }

    fclose(file);

    // -------- SHUFFLE WORDS (GUI Version) --------
    // This shuffles the order of words in the array so every 
    // playthrough presents words in a different sequence.
    if (word_count > 1) {
        for (int i = word_count - 1; i > 0; i--) {
            int j = rand() % (i + 1);
            char temp[MAX_LENGTH];
            
            // Swap words[i] and words[j]
            strcpy(temp, words[i]);
            strcpy(words[i], words[j]);
            strcpy(words[j], temp);
        }
    }
}

// -------- SCRAMBLE INDIVIDUAL WORD --------
void scramble_word(char word[]) {
    int len = strlen(word);
    char temp[MAX_LENGTH];
    strcpy(temp, word);

    if (len <= 1) return;

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
    if (current_index >= word_count) {
        current_index = 0; // Loop back to the start of the shuffled list
    }

    strcpy(current_word, words[current_index]);
    strcpy(scrambled, current_word);
    scramble_word(scrambled);

    input_index = 0;
    input[0] = '\0';
    showing_feedback = 0;

    // Setup letter buttons
    int len = strlen(scrambled);
    int total_width = len * (BUTTON_SIZE + BUTTON_SPACING) - BUTTON_SPACING;
    int start_x = (gfx_xsize() - total_width) / 2;
    int start_y = 450;
    
    button_count = len;
    for (int i = 0; i < len; i++) {
        letter_buttons[i].letter = scrambled[i];
        letter_buttons[i].x = start_x + i * (BUTTON_SIZE + BUTTON_SPACING);
        letter_buttons[i].y = start_y;
        letter_buttons[i].width = BUTTON_SIZE;
        letter_buttons[i].height = BUTTON_SIZE;
        letter_buttons[i].clicked = 0;
    }

    // Button positions
    submit_btn.x = 100; submit_btn.y = 520;
    submit_btn.w = 120; submit_btn.h = 40;

    back_btn.x = 240; back_btn.y = 520;
    back_btn.w = 120; back_btn.h = 40;

    reset_btn.x = 380; reset_btn.y = 520;
    reset_btn.w = 120; reset_btn.h = 40;

    quit_btn.x = 20; quit_btn.y = 20;
    quit_btn.w = 80; quit_btn.h = 30;

    start_time = time(NULL);
    current_index++;
}

// -------- INIT GAME --------
void game_init() {
    srand(time(NULL));
    load_words(); // This now loads AND shuffles the word list

    score = 0;
    lives = 3;
    current_index = 0;
    showing_feedback = 0;

    next_word();
}

// -------- DRAW GAME --------
void draw_game() {
    int w = gfx_xsize();
    int h = gfx_ysize();
    
    gfx_color(0,0,0);

    // Quit Button
    gfx_color(150, 0, 0);
    gfx_fillrectangle(quit_btn.x, quit_btn.y, quit_btn.w, quit_btn.h);
    gfx_color(255, 255, 255);
    gfx_text("QUIT", quit_btn.x + 20, quit_btn.y + 20, 1);

    gfx_color(0,0,0);
    gfx_text("WORD SCRAMBLE", w/2 - 120, 80, 2);

    // Info
    char info[100];
    sprintf(info, "Lives: %d    Score: %d", lives, score);
    gfx_text(info, w/2 - 120, 140, 1);

    // Feedback or Timer
    if (showing_feedback) {
        if (strstr(feedback_msg, "CORRECT")) gfx_color(0, 150, 0);
        else gfx_color(200, 0, 0);
        gfx_text(feedback_msg, w/2 - 120, 190, 1);
    } else {
        int time_left = time_limit - (time(NULL) - start_time);
        if (time_left < 0) time_left = 0;
        char timer[50];
        sprintf(timer, "Time Left: %d", time_left);
        if (time_left <= 5) gfx_color(255, 0, 0);
        else gfx_color(0, 0, 0);
        gfx_text(timer, w/2 - 55, 190, 1);
    }

    gfx_color(0,0,0);
    gfx_text("Scrambled:", w/2 - 100, 240, 1);
    gfx_text(scrambled, w/2 - 60, 280, 2);

    gfx_text("Your guess:", w/2 - 100, 340, 1);
    gfx_text(input, w/2 - 60, 380, 2);

    // Letter Buttons
    for (int i = 0; i < button_count; i++) {
        if (letter_buttons[i].clicked) gfx_color(180,180,180);
        else gfx_color(200,200,200);
        gfx_fillrectangle(letter_buttons[i].x, letter_buttons[i].y,
                          letter_buttons[i].width, letter_buttons[i].height);
        gfx_color(0,0,0);
        char str[2] = {letter_buttons[i].letter, '\0'};
        gfx_text(str, letter_buttons[i].x + 15, letter_buttons[i].y + 25, 1);
    }

    // Action Buttons
    gfx_color(100,200,100);
    gfx_fillrectangle(submit_btn.x, submit_btn.y, submit_btn.w, submit_btn.h);
    gfx_color(0,0,0);
    gfx_text("Submit", submit_btn.x + 30, submit_btn.y + 25, 1);

    gfx_color(200,100,100);
    gfx_fillrectangle(back_btn.x, back_btn.y, back_btn.w, back_btn.h);
    gfx_color(0,0,0);
    gfx_text("Delete", back_btn.x + 30, back_btn.y + 25, 1);

    gfx_color(200,200,50);
    gfx_fillrectangle(reset_btn.x, reset_btn.y, reset_btn.w, reset_btn.h);
    gfx_color(0,0,0);
    gfx_text("Reset", reset_btn.x + 35, reset_btn.y + 25, 1);
}

// -------- HANDLE INPUT --------
GameState game_handle_input(char c) {
    if (c != 1) return STATE_PLAYING;

    int mx = gfx_xpos();
    int my = gfx_ypos();

    if (mx >= quit_btn.x && mx <= quit_btn.x + quit_btn.w &&
        my >= quit_btn.y && my <= quit_btn.y + quit_btn.h) {
        return STATE_MENU;
    }

    if (showing_feedback) return STATE_PLAYING;

    for (int i = 0; i < button_count; i++) {
        LetterButton *b = &letter_buttons[i];
        if (!b->clicked && mx >= b->x && mx <= b->x + b->width &&
            my >= b->y && my <= b->y + b->height) {
            if (input_index < MAX_LENGTH - 1) {
                input[input_index++] = b->letter;
                input[input_index] = '\0';
                b->clicked = 1;
            }
            return STATE_PLAYING;
        }
    }

    if (mx >= submit_btn.x && mx <= submit_btn.x + submit_btn.w &&
        my >= submit_btn.y && my <= submit_btn.y + submit_btn.h) {
        input[input_index] = '\0';
        
        if (strcmp(input, current_word) == 0) {
            score += 10;
            sprintf(feedback_msg, "CORRECT! +10 Points");
        } else {
            lives--;
            sprintf(feedback_msg, "WRONG! It was: %s", current_word);
        }
        
        showing_feedback = 1;
        feedback_expiry = time(NULL) + 2;
        return STATE_PLAYING;
    }

    if (mx >= back_btn.x && mx <= back_btn.x + back_btn.w &&
        my >= back_btn.y && my <= back_btn.y + back_btn.h) {
        if (input_index > 0) {
            char to_remove = input[--input_index];
            input[input_index] = '\0';
            for (int i = button_count - 1; i >= 0; i--) {
                if (letter_buttons[i].clicked && letter_buttons[i].letter == to_remove) {
                    letter_buttons[i].clicked = 0;
                    break;
                }
            }
        }
        return STATE_PLAYING;
    }

    if (mx >= reset_btn.x && mx <= reset_btn.x + reset_btn.w &&
        my >= reset_btn.y && my <= reset_btn.y + reset_btn.h) {
        input_index = 0;
        input[0] = '\0';
        for (int i = 0; i < button_count; i++) letter_buttons[i].clicked = 0;
        return STATE_PLAYING;
    }

    return STATE_PLAYING;
}

// -------- UPDATE --------
GameState game_update() {
    if (showing_feedback) {
        if (time(NULL) >= feedback_expiry) {
            next_word();
        }
        return STATE_PLAYING;
    }

    int time_left = time_limit - (time(NULL) - start_time);
    if (time_left <= 0) {
        lives--;
        sprintf(feedback_msg, "TIME'S UP! It was: %s", current_word);
        showing_feedback = 1;
        feedback_expiry = time(NULL) + 2;
    }

    if (lives <= 0) {
        return STATE_MENU;
    }

    return STATE_PLAYING;
}
