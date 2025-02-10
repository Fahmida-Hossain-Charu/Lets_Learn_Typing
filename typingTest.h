#pragma once
#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

// Starts the typing test with a given passage length
void start_typing_test(int difficulty);

// Initializes color pairs for ncurses
void initialize_colors();

// Processes the user input and updates the relevant statistics
void process_input(char *text, char *input, size_t *position, int *word_count, int *correct_symbol_count, int *mistake_count, int *correct_word_count);

// Updates the screen with the current state of the typing test
void update_screen(char *text, char *input, size_t position, int correct_symbol_count, int correct_word_count, int word_count, int mistake_count, time_t start_time);

// Resets the input and prepares for the next passage
void reset_input(char *input, char *text, char *next_text, size_t *position, int words_per_line, int difficulty);

// Displays the final statistics after the typing test is completed
void display_final_stats(int correct_symbol_count, int correct_word_count, int mistake_count, time_t start_time);