#ifndef BOT_LOGIC
#define BOT_LOGIC

#include "chess_engine.h"
#include <time.h>
#include <stdlib.h>

int figure_value(char fig);
int evaluate_position(char cur_board[8][8][2], char color);
int minimax(char cur_board[8][8][2], char color, int depth);
int minimax_r(char cur_board[8][8][2], int flag, char color, int depth, int max_depth, int *best);
int position_bonus(char cur_board[8][8][2], char fig, int x, int y);

#endif
