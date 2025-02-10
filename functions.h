// Header file containing function declarations for text generation and custom printing.
#pragma once

void gen_text(char *text, int num_words, int difficulty);

void myPrint(int attrs, const char *format, ...);

void myMvPrint(int y, int x, int attrs, const char *format, ...);

int prompt(int diff);