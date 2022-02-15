#include "chess_engine.h"

int is_on_board(char cur_board[8][8][2], int x, int y) {
    return cur_board[y][x][0] != '0';
}

int check_turn(char cur_board[8][8][2], int a1, int a2, int b1, int b2)
{
    int x = b1 - a1, y = b2 - a2;
    char fig_t = cur_board[a2][a1][0], fig_c = cur_board[a2][a1][1];
    if (is_on_board(cur_board, b1, b2) && cur_board[b2][b1][1]==fig_c) return 0;
    
    if (fig_t=='p' && fig_c=='w'){
        if (y==1 && x==0 && (!is_on_board(cur_board, b1, b2))) return 1;
        if (y==2 && x==0 && a2==1 && (!is_on_board(cur_board, b1, b2)) && (!is_on_board(cur_board, b1, b2-1))) return 1;
        if (y==1 && (x==1 || x==-1) && is_on_board(cur_board, b1, b2)) return 1;
        /*if (y==1 && (x==1 || x==-1) && (!is_on_board(cur_board, b1, b2))) return 2;*/
    }

    if (fig_t=='p' && fig_c=='b'){
        if (y==-1 && x==0 && (!is_on_board(cur_board, b1, b2))) return 1;
        if (y==-2 && x==0 && a2==6 && (!is_on_board(cur_board, b1, b2)) && (!is_on_board(cur_board, b1, b2+1))) return 1;
        if (y==1 && (x==1 || x==-1) && is_on_board(cur_board, b1, b2)) return 1;
        /*if (y==1 && (x==1 || x==-1) && (!is_on_board(cur_board, b1, b2))) return 2;*/
    }

    if ((fig_t=='r' or fig_t=='q') and y==0){
        int flag = 1;
        for (int i = min(a1, b1)+1; i<max(a1,b1); ++i) flag &= !(is_on_board(cur_board, i, a2));
        if (flag) return 1;
    }
    if ((fig_t=='r' or fig_t=='q') and x==0){
        int flag = 1;
        for (int i = min(a2, b2)+1; i<max(a2,b2); ++i) flag &= !(is_on_board(cur_board, a1, i));
        if (flag) return 1;
    }
    if (fig_t=='b' or fig_t=='q'){
        if (x==y and x>0){
            int flag = 1;
            for (int i = 1; i<x; ++i) flag &= !(is_on_board(cur_board, a1+i, a2+i));
            if (flag) return 1;
        }
        if (x==y and x<0){
            int flag = 1;
            for (int i = 1; i<-x; ++i) flag &= !(is_on_board(cur_board, a1-i, a2-i));
            if (flag) return 1;
        }
        if (x==-y and x>0){
            int flag = 1;
            for (int i = 1; i<x; ++i) flag &= !(is_on_board(cur_board, a1+i, a2-i));
            if (flag) return 1;
        }
        if (x==-y and x<0){
            int flag = 1;
            for (int i = 1; i<-x; ++i) flag &= !(is_on_board(cur_board, a1-i, a2+i));
            if (flag) return 1;
        }
    }

    if (fig_t=='n'){
        if (abs(x)==2 and abs(y)==1 or abs(x)==1 and abs(y)==2) return 1;
    }
    if (fig_t=='k'){
        if (abs(x)<2 and abs(y)<2) return 1;
        if (fig_c=='w' and abs(x)==2 and y==0 and a2==0){
            int m1 = min(a1, b1);
            if (!is_on_board(cur_board, m1+1, 0) and !is_on_board(cur_board, m1+2, 0))
                if (x>0 and cur_board[0][7][0]=='r' and cur_board[0][7][1]=='w') return 3;
                if (x<0 and cur_board[0][0][0]=='r' and cur_board[0][0][1]=='w' and !is_on_board(cur_board,1,0)) return 3;
        }
        if (fig_c=='b' and abs(x)==2 and y==0 and a2==7){
            int m1 = min(a1, b1);
            if (!is_on_board(cur_board, m1+1, 7) and !is_on_board(cur_board, m1+2, 7))
                if (x>0 and cur_board[7][7][0]=='r' and cur_board[7][7][1]=='b') return 3;
                if (x<0 and cur_board[7][0][0]=='r' and cur_board[7][0][1]=='b' and !is_on_board(cur_board,1,7)) return 3;
        }
    }
    
    return 0;
}

int check_checkmate(char cur_board[8][8][2], int x, int y, char color) 
{
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (is_on_board(cur_board, j, i) and cur_board[i][j][1] != color and check_turn(cur_board, j, i, x, y)) {
                return 1;
            }
        }
    }
    return 0;
}

vector_int_t get_king_turns(int x, int y) {
    vector_int_t king_turns;
    v_init(&king_turns);
    if (x > 0 and y > 0) v_push(&king_turns, (x - 1)*10 + (y - 1));
    if (x > 0 and y < 7) v_push(&king_turns, (x - 1)*10 + (y + 1));
    if (x < 7 and y > 0) v_push(&king_turns, (x + 1)*10 + (y - 1));
    if (x < 7 and y < 7) v_push(&king_turns, (x + 1)*10 + (y + 1));
    if (x > 0) v_push(&king_turns, (x - 1)*10 + y);
    if (x < 7) v_push(&king_turns, (x + 1)*10 + y);
    if (y > 0) v_push(&king_turns, x*10 + (y - 1));
    if (y < 7) v_push(&king_turns, x*10 + (y + 1));
    return king_turns;
}

void delete_checkmates_for_king(char cur_board[8][8][2], vector_int_t king_turns, char color) 
{
    int c = 0;
    while (c != king_turns.len) {
        if (check_checkmate(cur_board, king_turns.data[c]/10, king_turns.data[c]%10, color)) {
            v_pop(&king_turns, c);
        } else {
            c++;
        }
    }
}

int king_position(char cur_board[8][8][2], char color)
{
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (cur_board[i][j][0]=='k' and cur_board[i][j][1]==color)
                return 10*j+i;
        }
    }
}

void write_turn(char cur_board[8][8][2], int a, int b)
{
    int a2 = a/10, a1 = a%10;
    int b2 = b/10, b1 = b%10;
    cur_board[b1][b2][0] = cur_board[a1][a2][0];
    cur_board[a1][a2][0] = '0';
    cur_board[b1][b2][1] = cur_board[a1][a2][1];
    cur_board[a1][a2][1] = '0';
}

int add_turn(char cur_board[8][8][2], int a, int b, char cur_color) 
{
    if (a<0 or b<0) return 0;
    int a1 = a/10, a2 = a%10;
    int b1 = b/10, b2 = b%10;
    int x = b1 - a1, y = b2 - a2;
    if (a1 < 0 or a1 > 7 or a2 < 0 or a2 > 7 or b1 < 0 or b1 > 7 or b2 < 0 or b2 > 7) return 0;
    if (!is_on_board(cur_board, a1, a2)) {
        return 0;
    }
    int turn = check_turn(cur_board, a1, a2, b1, b2);
    char color = cur_board[a2][a1][1];
    if (cur_color != color or turn==0) {
        return 0;
    }
    if (cur_board[a2][a1][0] == 'k' and turn != 3) {
        write_turn(cur_board, a, b);
        if (check_checkmate(cur_board, b1, b2, color)){
            write_turn(cur_board, b, a);
            return 0;
        }
    } else {
        int kp = king_position(cur_board, color);
        if (check_checkmate(cur_board, kp/10, kp%10, color)){
            write_turn(cur_board, a, b);
            if (check_checkmate(cur_board, kp/10, kp%10, color)){
                write_turn(cur_board, b, a);
                return 0;
            }
        } else {
            if (turn==1){
                write_turn(cur_board, a, b);
            } else if (turn==2){
                /*взятие на проходе*/
            } else if (turn==3) {
                if (color=='w'){
                    if (x>0){
                        a1 = 70;  a2 = 50;
                    } else {
                        a1 = 0; a2 = 30;
                    }
                } else {
                    if (x>0){
                        a1 = 77; a2 = 57;
                    } else {
                        a1 = 7; a2 = 37;
                    }
                }
                if (!check_checkmate(cur_board, b/10, b%10, color)) {
                    write_turn(cur_board, a1, a2);
                    write_turn(cur_board, a, b);
                }
            } else {
                return 0;
            }
        }
    }
    return 1;
}

void print_board(char cur_board[8][8][2])
{
    for (int i = 0; i<8; ++i){
        for (int j = 0; j<8; j++){
            printf("%c%c ", cur_board[7-i][j][0], cur_board[7-i][j][1]);
        }
        printf("\n");
    }
    printf("\n");
}

int from_str(char a[2])
{
    return ((int)a[0]-97)*10 + ((int)a[1]-48-1);
}

void copy_board(char board1[8][8][2], char board2[8][8][2])
{
    for (int i = 0; i<8; ++i)
        for (int j = 0; j<8; ++j)
            for (int k = 0; k<2; ++k) board2[i][j][k] = board1[i][j][k];
}

char* to_str(int a)
{
    char *b = malloc(2);
    b[0] = (char)(a/10+97);
    b[1] = (char)(a%10+48+1);
    return b;
}

vector_int_t turns(char cur_board[8][8][2], int a1)
{
    vector_int_t v;
    v_init(&v);
    char c_board[8][8][2];
    copy_board(cur_board, c_board);
    for (int i = 0; i<8; ++i)
        for (int j = 0; j<8; ++j){
            if (add_turn(c_board, a1, j*10+i, cur_board[a1%10][a1/10][1])){
                copy_board(cur_board, c_board);
                v_push(&v, j*10+i);
            }
        }
    return v;
}

void print_turns(char cur_board[8][8][2], vector_int_t v)
{
    char c_board[8][8][2];
    copy_board(cur_board, c_board);
    for (int i = 0; i<v.len; ++i) {
        c_board[v.data[i]%10][v.data[i]/10][0] = '*';
        c_board[v.data[i]%10][v.data[i]/10][1] = '*';
    }
    print_board(c_board);
}

int loss_q(char cur_board[8][8][2], char color)
{
    char c_board[8][8][2];
    copy_board(cur_board, c_board);
    for (int i = 0; i<8; ++i)
        for (int j = 0; j<8; ++j)
            if (cur_board[j][i][1]==color)
                for (int i1 = 0; i1<8; ++i1)
                    for (int j1 = 0; j1<8; ++j1)
                        if (add_turn(c_board, 10*j+i, 10*j1+i1, color)) return 0;  
                   
    return 1;
}


