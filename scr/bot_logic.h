#ifndef BOT_LOGIC
#define BOT_LOGIC

#include "chess_engine.h"

int figure_value(char fig);
int evaluate_position(char cur_board[8][8][2], char color);
int minimax(char cur_board[8][8][2], int flag, char color, int depth, int *best);

#endif
