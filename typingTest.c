#include "typing_test.h"
#include "ncurses.h"
#include "functions.h"

// Initialize color pairs for ncurses
void initialize_colors() {
    init_pair(1, COLOR_GREEN, -1);
    init_pair(2, COLOR_RED, -1);
    init_pair(3, COLOR_WHITE, -1);
    init_pair(4, COLOR_YELLOW, -1);
    init_pair(5, COLOR_MAGENTA, -1);
    init_pair(6, COLOR_CYAN, -1);
    init_pair(7, COLOR_BLUE, -1);
}

// Process user input and update the state accordingly
void process_input(char *text, char *input, size_t *pos, int *words, int *correct_symbols, int *mistakes_count, int *correct_words) {
    int ch = getch();
    
    // Handle ESC key to exit
    if (ch == 27) {
        endwin();
        exit(0);
    }
    if(ch == '5') {
        clear();
        refresh();
        int diff = prompt(ch-'0');
        // Clear the screen before starting the typing test
        clear();
        refresh();
        start_typing_test(diff);
    }
    // Handle Backspace key
    if (ch == 127 || ch == 8) {
        if (*pos > 0) {
            (*pos)--;
            if (text[*pos] == ' ') {
                (*words)++;
            } else if (input[*pos] == text[*pos]) {
                (*correct_symbols)--;
            } else {
                (*mistakes_count)--;
            }
            input[*pos] = '\0';
        }
    } else {
        // Handle normal character input
        if (text[*pos] == ' ' || text[*pos] == '\n') {
            (*words)--;
            size_t word_start = *pos;
            while (word_start > 0 && text[word_start - 1] != ' ') {
                word_start--;
            }
            if (strncmp(&input[word_start], &text[word_start], *pos - word_start) == 0) {
                (*correct_words)++;
            }
        } else if (ch == text[*pos]) {
            (*correct_symbols)++;
        } else {
            (*mistakes_count)++;
        }
        input[(*pos)++] = ch;
        input[*pos] = '\0';
    }
}

// Update the screen with the current state
void update_screen(char *text, char *input, size_t pos, int correct_symbols, int correct_words, int words, int mistakes_count, time_t start_time) {
    mvprintw(4, 0, "%-99s", " ");
    mvprintw(4, 0, "%s", input);

    // Display the text with color coding for correct/incorrect characters
    for (size_t j = 0; j < strlen(text); j++) {
        attron(input[j] == text[j] ? COLOR_PAIR(1) : COLOR_PAIR(2));
        mvaddch(1, j, text[j]);
        attroff(input[j] == text[j] ? COLOR_PAIR(1) : COLOR_PAIR(2));
    }

    mvchgat(1, pos, strlen(text) - pos, A_NORMAL, 3, NULL);
    mvchgat(1, pos, 1, A_NORMAL, 4, NULL);

    // Calculate and display typing statistics
    time_t time_now = time(NULL);
    double difference = difftime(time_now, start_time);
    double cpm = (double)correct_symbols / (difference / 60);
    double wpm = (double)correct_words / (difference / 60);
    myMvPrint(5, 0, COLOR_PAIR(1) | A_BOLD, "Words left: %d; Mistakes: %d; WPM: %.2lf; CPM: %.2lf\tTime: %.0lfs", words, mistakes_count, wpm, cpm, difference);
    refresh();
}

// Reset input and prepare for the next round of typing
void reset_input(char *input, char *text, char *next_text, size_t *pos, int words_per_line, int difficulty) {
    *pos = 0;
    strcpy(input, "");
    strcpy(text, next_text);
    gen_text(next_text, words_per_line, difficulty);

    clear();
    myMvPrint(0, 0, COLOR_PAIR(6), "Type this text:");
    mvprintw(1, 0, "%s", text);
    mvprintw(2, 0, "%s", next_text);
    myMvPrint(3, 0, COLOR_PAIR(6), "Your typing:");
    mvprintw(4, 0, "%-99s", " ");
}

// Display final statistics after the test is completed
void display_final_stats(int correct_symbols, int correct_words, int mistakes_count, time_t start_time) {
    time_t end_time = time(NULL);
    double test_duration = difftime(end_time, start_time);
    double accuracy = (correct_symbols + mistakes_count) > 0 ? ((double)correct_symbols / (correct_symbols + mistakes_count)) * 100.0 : 0.0;
    double cpm = test_duration > 0 ? (double)correct_symbols / (test_duration / 60.0) : 0.0;
    double wpm = test_duration > 0 ? (double)correct_words / (test_duration / 60.0) : 0.0;
    double cps = test_duration > 0 ? (double)correct_symbols / test_duration : 0.0;

    clear();
    move(0, 0);
    myPrint(COLOR_PAIR(4), "Test completed!\n");
    myPrint(COLOR_PAIR(7), "Total Keystrokes: ");
    myPrint(COLOR_PAIR(1), "(%d", correct_symbols);
    myPrint(COLOR_PAIR(2), "|%d)\n", mistakes_count);
    printw("Correct characters: %d\n", correct_symbols);
    printw("Mistakes: %d\n", mistakes_count);
    printw("Accuracy: %.2lf\n", accuracy);
    printw("Words per minute (WPM): %.2lf\n", wpm);
    printw("Characters per minute (CPM): %.2lf\n", cpm);
    printw("Characters per second (CPS): %.2lf\n", cps);
    printw("Test duration: %.0lf seconds\n", test_duration);
    myPrint(COLOR_PAIR(2), "Press Esc to exit or Enter to restart the game.\n");
    refresh();

    // Wait for user input to either exit or restart the game
    while (1) {
        int ch = getch();
        if (ch == 27) { // ESC key
            break;
        } else if (ch == 10) { // Enter key
            clear();
            myPrint(COLOR_PAIR(3), "Select difficulty:\n");
            myPrint(COLOR_PAIR(4), "1. Easy (30 words)\n");
            myPrint(COLOR_PAIR(5), "2. Medium (60 words)\n");
            myPrint(COLOR_PAIR(6), "3. Hard (90 words)\n");
            myPrint(COLOR_PAIR(4), "Press 1, 2 or 3: ");
            refresh();
            while (1) {
                ch = getch();
                if (ch == 27) {
                    endwin();
                    exit(0);
                } else if (ch == '1' || ch == '2' || ch == '3') {
                    break;
                }
                else if(ch == '5') {
                    clear();
                    refresh();
                    int diff = prompt(ch-'0');
                    // Clear the screen before starting the typing test
                    clear();
                    refresh();
                    start_typing_test(diff);
                }
                else {
                    clear();
                    myPrint(COLOR_PAIR(2), "Invalid input. Please select difficulty:\n");
                    myPrint(COLOR_PAIR(4), "1. Easy (30 words)\n");
                    myPrint(COLOR_PAIR(5), "2. Medium (60 words)\n");
                    myPrint(COLOR_PAIR(6), "3. Hard (90 words)\n");
                    myPrint(COLOR_PAIR(4), "Press 1, 2 or 3: ");
                    refresh();
                }
            }
            clear();
            refresh();
            start_typing_test(ch - '0');
        }
    }
}

// Start the typing test with the given difficulty level
void start_typing_test(int difficulty) {
    char text[100] = "";
    char next_text[100] = "";
    char input[100] = "";
    int words;
    int correct_symbols = 0;
    int mistakes_count = 0;
    int correct_words = 0;
    int words_per_line = 5;
    size_t pos = 0;
    int total_word_cnt = 0;

    initialize_colors();

    // Display welcome message and difficulty level
    myPrint(COLOR_PAIR(3), "Welcome to the typing test!\n");
    myPrint(COLOR_PAIR(3), "Press ESC to exit or 5 to restart the game at any time.\n");
    myPrint(COLOR_PAIR(3), "Difficulty: ");
    if (difficulty == 1) {
        myPrint(COLOR_PAIR(3), "Easy\n");
        words = 30;
    } else if (difficulty == 2) {
        myPrint(COLOR_PAIR(3), "Medium\n");
        words = 60;
    } else if (difficulty == 3) {
        myPrint(COLOR_PAIR(3), "Hard\n");
        words = 90;
    }
    myPrint(COLOR_PAIR(4), "Press Enter to start the game.");
    
    // Wait for Enter key to start the game
    char ch = getch();
    if (ch == 27) {
        endwin();
        exit(0);
    }
    if (ch == '5') {
        clear();
        refresh();
        int diff = prompt(ch-'0');
        // Clear the screen before starting the typing test
        clear();
        refresh();
        start_typing_test(diff);
    } 
    if(ch != 10) {
        myMvPrint(3, 0, COLOR_PAIR(2), "Invalid input. Press Enter to start the game.");
        ch = getch();
        if(ch == 27) {
            endwin();
            exit(0);
        }
        if (ch == '5') {
            clear();
            refresh();
            int diff = prompt(ch-'0');
            // Clear the screen before starting the typing test
            clear();
            refresh();
            start_typing_test(diff);
        }
        while(ch != 10) {
            ch = getch();
            if(ch == 27) {
                endwin();
                exit(0);
            }
        }
    }
    clear();
    refresh();
    move(0, 0);
    clrtoeol();

    // Generate initial text for typing
    gen_text(text, words_per_line, difficulty);
    gen_text(next_text, words_per_line, difficulty);
    myMvPrint(0, 0, COLOR_PAIR(6), "Type this text:");
    mvprintw(1, 0, "%s", text);
    mvprintw(2, 0, "%s", next_text);
    myMvPrint(3, 0, COLOR_PAIR(6), "Your typing:");

    time_t start_time = time(NULL);

    // Main typing loop
    while (words > 0) {
        process_input(text, input, &pos, &words, &correct_symbols, &mistakes_count, &correct_words);
        update_screen(text, input, pos, correct_symbols, correct_words, words, mistakes_count, start_time);

        // Check if the current text has been fully typed
        if (pos >= strlen(text)) {
            total_word_cnt += words_per_line;
            if ((difficulty == 1 && total_word_cnt > 30) || (difficulty == 2 && total_word_cnt > 60) || (difficulty == 3 && total_word_cnt > 90)) {
                break;
            }
            reset_input(input, text, next_text, &pos, words_per_line, difficulty);
            update_screen(text, input, pos, correct_symbols, correct_words, words, mistakes_count, start_time);
        }

        // End the test if 60 seconds have passed
        if (difftime(time(NULL), start_time) >= 60) {
            break;
        }
    }

    // Display final statistics
    display_final_stats(correct_symbols, correct_words, mistakes_count, start_time);
}