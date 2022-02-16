#include "bot_logic.h"

int figure_value(char fig)
{
    switch (fig)
    {
    case 'p': return 10;
    case 'n': return 30;
    case 'b': return 30;
    case 'r': return 50;
    case 'q': return 90;
    case 'k': return 900;
    default: {printf("Error 1"); return -1000000;}
    }
}

int evaluate_position(char cur_board[8][8][2], char color)
{
    int sum = 0;
    char enemy = color=='w'?'b':'w';
    for (int i = 0; i<8; ++i)
        for (int j = 0; j<8; ++j)
            if (cur_board[i][j][1]==color) sum+=figure_value(cur_board[i][j][0]);
            else if (cur_board[i][j][1]==enemy) sum-=figure_value(cur_board[i][j][0]);
    return sum;
}

int minimax(char cur_board[8][8][2], char color, int depth)
{
    int res = -1;
    minimax_r(cur_board, 1, color, depth, depth, &res);
    return res;
}

int minimax_r(char cur_board[8][8][2], int flag, char color, int depth, int max_depth, int *best)
{
    if (depth==0){
        int temp = evaluate_position(cur_board, color);
        return temp;
    }
    if (flag){
        int best_move = -1000000;
        for (int i = 0; i<8; ++i)
            for (int j = 0; j<8; ++j)
                if (cur_board[i][j][1]==color){
                    vector_int_t t = turns(cur_board, 10*j+i);
                    for (int k = 0; k<t.len; ++k){
                        char temp_board[8][8][2];
                        copy_board(cur_board, temp_board);
                        write_turn(temp_board, 10*j+i, t.data[k]);
                        int bq = minimax_r(temp_board, !flag, color=='w'?'b':'w', depth-1, max_depth, best);
                        if (bq>best_move){
                            best_move = bq;
                            if (depth==max_depth) *best = 100*(10*j+i)+t.data[k];
                        } else if (bq==best_move && depth==max_depth){
                            srand(time(NULL));
                            int r = rand()%3;
                            if (r) *best = 100*(10*j+i)+t.data[k];
                        }
                    }
                }
        return best_move;
    } else {
        int best_move = 1000000;
        for (int i = 0; i<8; ++i)
            for (int j = 0; j<8; ++j)
                if (cur_board[i][j][1]==color){
                    vector_int_t t = turns(cur_board, 10*j+i);
                    for (int k = 0; k<t.len; ++k){
                        char temp_board[8][8][2];
                        copy_board(cur_board, temp_board);
                        write_turn(temp_board, 10*j+i, t.data[k]);
                        int bq = minimax_r(temp_board, !flag, color=='w'?'b':'w', depth-1, max_depth, best);
                        if (bq<best_move){
                            best_move = bq;
                            if (depth==max_depth) *best = 100*(10*j+i)+t.data[k];
                        }  else if (bq==best_move && depth==max_depth){
                            srand(time(NULL));
                            int r = rand()%3;
                            if (r) *best = 100*(10*j+i)+t.data[k];
                        }
                    }
                }
        return best_move;
    }
}


