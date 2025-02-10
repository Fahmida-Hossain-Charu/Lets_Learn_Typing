#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <ncurses.h>
#include "functions.h"
#include "data.h"

/**
 * Generates a text string composed of random words based on the specified difficulty level.
 * each line will have 5 words.
 */
void gen_text(char *text, int num_words, int difficulty) {
    int size = sizeof(words) / sizeof(words[0]);
    strcpy(text, ""); // Initialize the text buffer

    for (int i = 0; i < num_words; ++i) {
        char word[20];
        strcpy(word, words[rand() % size]); // Select a random word

        if (difficulty == 1) {
            // Easy: Words with length 1 to 4
            if (strlen(word) >= 1 && strlen(word) < 5) {
                strcat(text, word);
                strcat(text, " ");
            } else {
                i--; // Retry if word doesn't match difficulty
            }
        } else if (difficulty == 2) {
            // Medium: Words with length 5 to 7
            if (strlen(word) >= 5 && strlen(word) < 8) {
                strcat(text, word);
                strcat(text, " ");
            } else {
                i--; // Retry if word doesn't match difficulty
            }
        } else if (difficulty == 3) {
            // Hard: Words with length 8 or more
            if (strlen(word) >= 8) {
                strcat(text, word);
                strcat(text, " ");
            } else {
                i--; // Retry if word doesn't match difficulty
            }
        }
    }

    strcat(text, "\n"); // Add a newline at the end
    text[strlen(text) - 1] = '\0'; // null-terminator
}

//Prints formatted text with the specified attribute.

void myPrint(int attribute, const char *format, ...) {
    attron(attribute); // Turn on the attribute
    va_list args;
    va_start(args, format);
    vw_printw(stdscr, format, args); // Print the formatted string
    va_end(args);
    attroff(attribute); // Turn off the attribute
}

//Moves the cursor to the specified position and prints formatted text with the specified attributes.

void myMvPrint(int y, int x, int attrs, const char *format, ...) {
    attron(attrs); // Turn on the attributes
    va_list args; 
    va_start(args, format);
    move(y, x); // Move the cursor
    vw_printw(stdscr, format, args); // Print the formatted string
    va_end(args);
    attroff(attrs); // Turn off the attributes
}

int prompt(int ch){
    do {
        // Clear the screen and display difficulty selection menu
        clear();
        attron(COLOR_PAIR(3));
        printw("Select difficulty:\n");
        attroff(COLOR_PAIR(3));
        attron(COLOR_PAIR(4));
        printw("1. Easy (30 words)\n");
        attroff(COLOR_PAIR(4));
        attron(COLOR_PAIR(5));
        printw("2. Medium (60 words)\n");
        attroff(COLOR_PAIR(5));
        attron(COLOR_PAIR(6));
        printw("3. Hard (90 words)\n");
        attroff(COLOR_PAIR(6));
        attron(COLOR_PAIR(3));
        printw("Press 1, 2 or 3: ");
        attroff(COLOR_PAIR(3));
        refresh();
        ch = getch();
        // Exit if ESC key is pressed
        if(ch == 27) {
            endwin();
            exit(0);
        }
    } while (ch != '1' && ch != '2' && ch != '3');
    return ch - '0';
}