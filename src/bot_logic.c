#include "bot_logic.h"

/*
    функция добавляющая позиционную оценку фигурам
*/
int position_bonus(char cur_board[8][8][2], char fig, int x, int y)
{
    if (fig=='p'){
        char p[8][8] = {{0,  0,  0,  0,  0,  0,  0,  0 },
                        {50, 50, 50, 50, 50, 50, 50, 50},
                        {10, 10, 20, 30, 30, 20, 10, 10},
                        {5,  5,  10, 25, 25, 10,  5, 5 },
                        {0,  0,  0,  31, 31,  0,  0, 0 },
                        {5, -5,-10,  0,  0, -10, -5, 5 },
                        {5,  10, 10,-20,-20, 10, 10, 5 },
                        {0,  0,  0,  0,  0,  0,  0,  0}};
        return p[7-y][x];
    }
    if (fig=='n'){
        char n[8][8] = {{-50,-40,-30,-30,-30,-30,-40,-50},
                        {-40,-20,  0,  0,  0,  0,-20,-40},
                        {-30,  0, 10, 15, 15, 10,  0,-30},
                        {-30,  5, 15, 20, 20, 15,  5,-30},
                        {-30,  0, 15, 20, 20, 15,  0,-30},
                        {-30,  5, 10, 15, 15, 10,  5,-30},
                        {-40,-20,  0,  5,  5,  0,-20,-40},
                        {-50,-40,-30,-30,-30,-30,-40,-50}};
        return n[7-y][x];
    }
    if (fig=='b'){
        char b[8][8] = {{-20,-10,-10,-10,-10,-10,-10,-20},
                        {-10,  0,  0,  0,  0,  0,  0,-10},
                        {-10,  0,  5, 10, 10,  5,  0,-10},
                        {-10,  5,  5, 10, 10,  5,  5,-10},
                        {-10,  0, 10, 10, 10, 10,  0,-10},
                        {-10, 10, 10, 10, 10, 10, 10,-10},
                        {-10,  5,  0,  0,  0,  0,  5,-10},
                        {-20,-10,-10,-10,-10,-10,-10,-20}};
        return b[7-y][x];
    }
    if (fig=='r'){
        char r[8][8] = {{0, 0, 0, 0, 0, 0, 0, 0},
                        {5, 10, 10, 10, 10, 10, 10,  5},
                        {-5,  0,  0,  0,  0,  0,  0, -5},
                        {-5,  0,  0,  0,  0,  0,  0, -5},
                        {-5,  0,  0,  0,  0,  0,  0, -5},
                        {-5,  0,  0,  0,  0,  0,  0, -5},
                        {-5,  0,  0,  0,  0,  0,  0, -5},
                        {0,  0,  0,  5,  5,  0,  0,  0}};

        return r[7-y][x];
    }
    if (fig=='q'){
        char q[8][8] = {{-20,-10,-10, -5, -5,-10,-10,-20},
                        {-10,  0,  0,  0,  0,  0,  0,-10},
                        {-10,  0,  5,  5,  5,  5,  0,-10},
                        {-5,  0,  5,  5,  5,  5,  0, -5 },
                        { 0,  0,  5,  5,  5,  5,  0, -5},
                        {-10,  5,  5,  5,  5,  5,  0,-10},
                        {-10,  0,  5,  0,  0,  0,  0,-10},
                        {-20,-10,-10, -5, -5,-10,-10,-20}};
        return q[7-y][x];
    }
    if (fig=='k'){
        char k[8][8] = {{-30,-40,-40,-50,-50,-40,-40,-30},
                        {-30,-40,-40,-50,-50,-40,-40,-30},
                        {-30,-40,-40,-50,-50,-40,-40,-30},
                        {-30,-40,-40,-50,-50,-40,-40,-30},
                        {-20,-30,-30,-40,-40,-30,-30,-20},
                        {-10,-20,-20,-20,-20,-20,-20,-10},
                        {20, 20,  0,  0,  0,  0, 20, 20},
                        {20, 30, 10,  0,  0, 10, 30, 20}};
        return k[7-y][x];
     }
     return 0;
}


/*
    функция оценки каждой фигуры отдельно
*/
int figure_value(char fig)
{
    switch (fig){
    case 'p': return 100;
    case 'n': return 320;
    case 'b': return 330;
    case 'r': return 500;
    case 'q': return 900;
    case 'k': return 20000;
    default: {printf("Error 1\n"); return INT_MIN;}
    }
}

/*
    функция оценки всей доски в целом
*/
int evaluate_position(char cur_board[8][8][2], char color)
{
    int sum = 0;
    char enemy = color=='w'?'b':'w';
    for (int i = 0; i<8; ++i)
        for (int j = 0; j<8; ++j)
            if (cur_board[i][j][1]==color){
                sum+=figure_value(cur_board[i][j][0]);
                if (color=='w')
                    sum+=position_bonus(cur_board, cur_board[i][j][0], j, i);
                if (color=='b')
                    sum+=position_bonus(cur_board, cur_board[i][j][0], 7-j, 7-i);
            }
            else if (cur_board[i][j][1]==enemy){
                sum-=figure_value(cur_board[i][j][0]);
                if (color=='w')
                    sum-=position_bonus(cur_board, cur_board[i][j][0], j, i);
                if (color=='b')
                    sum-=position_bonus(cur_board, cur_board[i][j][0], 7-j, 7-i);
            }
    return sum;
}

/*
    функция, запускающая рекурсивный алгоритм минимакс (с альфа/бета отсечениями)
*/
int minimax(char cur_board[8][8][2], char color, int depth)
{
    int res = -1;
    minimax_with_a_b(cur_board, 1, color, depth, depth, &res, INT_MIN, INT_MAX);
    return res;
}

/*
    алгоритм минимакс без альфа/бета отсечений и с дополнительными расходами памяти
*/
int minimax_r(char cur_board[8][8][2],
              int flag, char color,
              int depth, int max_depth,
              int *best)
{
    if (depth==0){
        int temp = evaluate_position(cur_board, color);
        return temp;
    }
    if (flag){
        int best_move = INT_MIN;
        for (int i = 0; i<8; ++i)
            for (int j = 0; j<8; ++j)
                if (cur_board[i][j][1]==color){
                    vector_int_t t = turns(cur_board, 10*j+i);
                    for (int k = 0; k<t.len; ++k){
                        char temp_board[8][8][2];
                        copy_board(cur_board, temp_board);
                        write_turn(temp_board, 10*j+i, t.data[k]);
                        int bq = minimax_r(temp_board,
                                           !flag, color=='w'?'b':'w',
                                           depth-1, max_depth,
                                           best
                                           );
                        if (bq>best_move){
                            best_move = bq;
                            if (depth==max_depth) *best = 100*(10*j+i)+t.data[k];
                        } else if (bq==best_move && depth==max_depth){
                            srand(time(NULL));
                            int r = rand()%3;
                            if (r) *best = 100*(10*j+i)+t.data[k];
                        }
                    }
                    v_clear(&t);
                }
        return best_move;
    } else {
        int best_move = INT_MAX;
        for (int i = 0; i<8; ++i)
            for (int j = 0; j<8; ++j)
                if (cur_board[i][j][1]==color){
                    vector_int_t t = turns(cur_board, 10*j+i);
                    for (int k = 0; k<t.len; ++k){
                        char temp_board[8][8][2];
                        copy_board(cur_board, temp_board);
                        write_turn(temp_board, 10*j+i, t.data[k]);
                        int bq = minimax_r(temp_board,
                                           !flag, color=='w'?'b':'w',
                                           depth-1, max_depth,
                                           best
                                           );
                        if (bq<best_move){
                            best_move = bq;
                            if (depth==max_depth) *best = 100*(10*j+i)+t.data[k];
                        }  else if (bq==best_move && depth==max_depth){
                            srand(time(NULL));
                            int r = rand()%3;
                            if (r) *best = 100*(10*j+i)+t.data[k];
                        }
                    }
                    v_clear(&t);
                }
        return best_move;
    }
}

/*
    алгоритм минимакс с альфа/бета отсечениями
*/
int minimax_with_a_b(char cur_board[8][8][2],
                     int flag, char color,
                     int depth, int max_depth,
                     int *best,
                     int alpha, int beta)
{
    if (depth==0){
        int temp = evaluate_position(cur_board, color);
        return temp;
    }
    int best_move = flag? INT_MIN : INT_MAX;
    for (int i = 0; i<8 && beta>=alpha; ++i)
        for (int j = 0; j<8 && beta>=alpha; ++j)
            if (cur_board[i][j][1]==color){
                vector_int_t t = turns(cur_board, 10*j+i);
                for (int k = 0; k<t.len && beta>=alpha; ++k) {
                    int t_d = t.data[k];
                    int roque = check_turn(cur_board, j, i, t_d/10, t_d%10)==3;
                    char f11 = cur_board[i][j][0], f21 = cur_board[t_d%10][t_d/10][0];
                    char f12 = cur_board[i][j][1], f22 = cur_board[t_d%10][t_d/10][1];
                    write_turn(cur_board, 10*j+i, t_d);
                    int bq = minimax_with_a_b(cur_board,
                                              !flag, color=='w'?'b':'w',
                                              depth-1, max_depth,
                                              best,
                                              alpha, beta
                                              );
                    if ((bq>best_move)&&flag || (!flag)&&(bq<best_move)){
                        best_move = bq;
                        if (depth==max_depth) *best = 100*(10*j+i)+t_d;
                        if (bq==best_move && depth==max_depth){
                            srand(time(NULL));
                            if (rand()%3) *best = 100*(10*j+i)+t_d;
                        }
                    }

                    cur_board[i][j][0] = f11;
                    cur_board[i][j][1] = f12;
                    cur_board[t_d%10][t_d/10][0] = f21;
                    cur_board[t_d%10][t_d/10][1] = f22;
                    if (roque){
                        int r = find_rock(cur_board, color, t_d/10-j);
                        int r1 = r/100, r2 = r%100;
                        cur_board[r1%10][r1/10][0] = '0';
                        cur_board[r1%10][r1/10][1] = '0';
                        cur_board[r2%10][r2/10][0] = 'r';
                        cur_board[r2%10][r2/10][1] = color;
                    }

                    if (flag){
                        alpha = alpha<bq? bq : alpha;
                    } else {
                        beta = beta<bq? beta: bq;
                    }
                }
                v_clear(&t);
            }
        return best_move;
}

