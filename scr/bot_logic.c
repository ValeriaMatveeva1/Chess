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
    for (int i = 0; i<8; ++i)
        for (int j = 0; j<8; ++j)
            if (cur_board[i][j][1]==color) sum+=figure_value(cur_board[i][j][0]);
    return sum;
}

int minimax(char cur_board[8][8][2], int flag, char color, int depth, int *best)
{
    if (depth==0){
        int temp = -evaluate_position(cur_board, color);
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
                        int bq = minimax(temp_board, !flag, color=='w'?'b':'w', depth-1, best);
                        if (bq>best_move){
                            best_move = bq;
                            *best = 100*(10*j+i)+t.data[k];
                            //printf("%c%c -> %c%c\n", to_str(10*j+i)[0], to_str(10*j+i)[1], to_str(t.data[k])[0], to_str(t.data[k])[1]);
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
                        int bq = minimax(temp_board, !flag, color=='w'?'b':'w', depth-1, best);
                        if (bq<best_move){
                            best_move = bq;
                            *best = 100*(10*j+i)+t.data[k];
                            //printf("!%c%c -> %c%c\n", to_str(10*j+i)[0], to_str(10*j+i)[1], to_str(t.data[k])[0], to_str(t.data[k])[1]);
                        }
                    }
                }
        return best_move;
    }
}


