#include "chess_engine.h"
#include "bot_logic.h"
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#define win_len 1000
#define win_height 700
#define box_len 80


void game_restart();
int from_win_coord(int x, int y);
char* long_fig_name(char short_name[2]);
void re_draw_board();
void draw_turn(int player_turn);
LRESULT WINAPI DefWindProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
