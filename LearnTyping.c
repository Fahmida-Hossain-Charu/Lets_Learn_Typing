#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include "typing_test.h"
#include "data.h"
#include "functions.h"
#include <stdio.h>

int main() {
    // Load word pool from redirected file (./game < word_pool.txt)
    load_word_pool();
    // Redirect stdin to the terminal
    freopen("/dev/tty", "r", stdin);
    
    // Initialize ncurses
    initscr();
    noecho();
    cbreak();
    start_color();
    use_default_colors();
    curs_set(0);
    srand(time(NULL));

    // Initialize color pairs
    init_pair(1, COLOR_GREEN, -1);
    init_pair(2, COLOR_RED, -1);
    init_pair(3, COLOR_WHITE, -1);
    init_pair(4, COLOR_YELLOW, -1);
    init_pair(5, COLOR_MAGENTA, -1);
    init_pair(6, COLOR_CYAN, -1);

    int ch;
    int diff= prompt(ch);
    // Clear the screen before starting the typing test
    clear();
    refresh();

    // Start the typing test with the selected difficulty
    start_typing_test(diff);

    // End ncurses mode
    endwin();
    return 0;
}