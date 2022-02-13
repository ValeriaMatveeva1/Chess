#ifndef CHESS_ENGINE
#define CHESS_ENGINE

#include "vector.h"
#include <stdio.h>
#include <stdlib.h>
#define or ||
#define and &&
#define min(a, b) ((a)>(b) ? (b):(a))
#define max(a, b) ((a)<(b) ? (b):(a))
#define abs(a) ((a)>0 ? (a): -(a))

int is_on_board(char cur_board[8][8][2], int x, int y);
int check_turn(char cur_board[8][8][2], int a1, int a2, int b1, int b2);
int check_checkmate(char cur_board[8][8][2], int x, int y, char color);
vector_int_t get_king_turns(int x, int y);
void delete_checkmates_for_king(char cur_board[8][8][2], vector_int_t king_turns, char color);
int king_position(char cur_board[8][8][2], char color);
void write_turn(char cur_board[8][8][2], int a, int b);
int add_turn(char cur_board[8][8][2], int a, int b, char cur_color);
void print_board(char cur_board[8][8][2]);
int from_str(char a[2]);
void copy_board(char board1[8][8][2], char board2[8][8][2]);
char* to_str(int a);
vector_int_t turns(char cur_board[8][8][2], int a1);
void print_turns(char cur_board[8][8][2], vector_int_t v);
int loss_q(char cur_board[8][8][2], char color);

#endif