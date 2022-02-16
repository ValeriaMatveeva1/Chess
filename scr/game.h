#ifndef GAME
#define GAME

#include "chess_engine.h"
#include "bot_logic.h"
#include <stdlib.h>

int ask_turn();
void write_bot_turn(char cur_board[8][8][2], char color);
void game(char color);

#endif
