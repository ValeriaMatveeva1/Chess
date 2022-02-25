#include "chess_engine.h"
/*
    все ходы хранятся в виде числового представления, в виде xy,
    где x = кордината по оси х (от 0 до 7) и в символьном формате принимает значения:
    a, b, c, d, e, f, g, h
    y - кордината по оси y (от 0 до 7), которая в символьном формате принимает значения
    1, 2, 3, 4, 5, 6, 7, 8
*/


/*
    функция проверки наличи фигуры на доске
*/
int is_on_board(char cur_board[8][8][2], int x, int y)
{
    return cur_board[y][x][0] != '0';
}

/*
    функция, проверяющая ход
    0 - ход сделать нельзя
    1 - можно
    3 - рокировка
*/
int check_turn(char cur_board[8][8][2], int a1, int a2, int b1, int b2)
{
    int x = b1 - a1, y = b2 - a2;
    char fig_t = cur_board[a2][a1][0], fig_c = cur_board[a2][a1][1];
    if (is_on_board(cur_board, b1, b2) && cur_board[b2][b1][1]==fig_c)
        return 0;

    if (fig_t=='p' && fig_c=='w'){
        if (y==1 && x==0 && (!is_on_board(cur_board, b1, b2)))
            return 1;
        if (y==2 && x==0 && a2==1 && (!is_on_board(cur_board, b1, b2)) && (!is_on_board(cur_board, b1, b2-1)))
            return 1;
        if (y==1 && (x==1 || x==-1) && is_on_board(cur_board, b1, b2))
            return 1;
        /*if (y==1 && (x==1 || x==-1) && (!is_on_board(cur_board, b1, b2))) return 2;*/
    }

    if (fig_t=='p' && fig_c=='b'){
        if (y==-1 && x==0 && (!is_on_board(cur_board, b1, b2)))
            return 1;
        if (y==-2 && x==0 && a2==6 && (!is_on_board(cur_board, b1, b2)) && (!is_on_board(cur_board, b1, b2+1)))
            return 1;
        if (y==-1 && (x==1 || x==-1) && is_on_board(cur_board, b1, b2))
            return 1;
        /*if (y==1 && (x==1 || x==-1) && (!is_on_board(cur_board, b1, b2))) return 2;*/
    }

    if ((fig_t=='r' || fig_t=='q') && y==0){
        int flag = 1;
        for (int i = min(a1, b1)+1; i<max(a1,b1); ++i)
            flag &= !(is_on_board(cur_board, i, a2));
        if (flag) return 1;
    }
    if ((fig_t=='r' || fig_t=='q') && x==0){
        int flag = 1;
        for (int i = min(a2, b2)+1; i<max(a2,b2); ++i)
            flag &= !(is_on_board(cur_board, a1, i));
        if (flag) return 1;
    }
    if (fig_t=='b' || fig_t=='q'){
        if (x==y && x>0){
            int flag = 1;
            for (int i = 1; i<x; ++i)
                flag &= !(is_on_board(cur_board, a1+i, a2+i));
            if (flag) return 1;
        }
        if (x==y && x<0){
            int flag = 1;
            for (int i = 1; i<-x; ++i)
                flag &= !(is_on_board(cur_board, a1-i, a2-i));
            if (flag) return 1;
        }
        if (x==-y && x>0){
            int flag = 1;
            for (int i = 1; i<x; ++i)
                flag &= !(is_on_board(cur_board, a1+i, a2-i));
            if (flag) return 1;
        }
        if (x==-y && x<0){
            int flag = 1;
            for (int i = 1; i<-x; ++i)
                flag &= !(is_on_board(cur_board, a1-i, a2+i));
            if (flag) return 1;
        }
    }

    if (fig_t=='n'){
        if ((abs(x)==2 && abs(y)==1) || (abs(x)==1 && abs(y)==2))
            return 1;
    }
    if (fig_t=='k'){
        if (abs(x)<2 && abs(y)<2)
            return 1;
        if (fig_c=='w' && abs(x)==2 && y==0 && a2==0 && a1==4){
            if (x>0 && !is_on_board(cur_board, 5, 0) && !is_on_board(cur_board, 6, 0))
                if (cur_board[0][7][0]=='r' && cur_board[0][7][1]=='w')
                    return 3;
            if (x<0 && !is_on_board(cur_board, 1, 0) && !is_on_board(cur_board, 2, 0) && !is_on_board(cur_board, 3, 0))
                if (cur_board[0][0][0]=='r' && cur_board[0][0][1]=='w')
                    return 3;

        }
        if (fig_c=='b' && abs(x)==2 && y==0 && a2==7 && a1==4){
            if (x>0 && !is_on_board(cur_board, 5, 7) && !is_on_board(cur_board, 6, 7))
                if (cur_board[7][7][0]=='r' && cur_board[7][7][1]=='b')
                    return 3;
            if (x<0 && !is_on_board(cur_board, 1, 7) && !is_on_board(cur_board, 2, 7) && !is_on_board(cur_board, 3, 7))
                if (cur_board[7][0][0]=='r' && cur_board[7][0][1]=='b')
                    return 3;

        }
    }

    return 0;
}

/*
    функция проврки наличия шаха у игрока с цветом color
*/
int check_checkmate(char cur_board[8][8][2], int x, int y, char color)
{
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (is_on_board(cur_board, j, i) && cur_board[i][j][1] != color && check_turn(cur_board, j, i, x, y)!=0) {
                return 1;
            }
        }
    }
    return 0;
}

/*
    список всех ходов короля
*/
vector_int_t get_king_turns(int x, int y) {
    vector_int_t king_turns;
    v_init(&king_turns);
    if (x > 0 && y > 0)
        v_push(&king_turns, (x - 1)*10 + (y - 1));
    if (x > 0 && y < 7)
        v_push(&king_turns, (x - 1)*10 + (y + 1));
    if (x < 7 && y > 0)
        v_push(&king_turns, (x + 1)*10 + (y - 1));
    if (x < 7 && y < 7)
        v_push(&king_turns, (x + 1)*10 + (y + 1));
    if (x > 0)
        v_push(&king_turns, (x - 1)*10 + y);
    if (x < 7)
        v_push(&king_turns, (x + 1)*10 + y);
    if (y > 0)
        v_push(&king_turns, x*10 + (y - 1));
    if (y < 7)
        v_push(&king_turns, x*10 + (y + 1));
    return king_turns;
}

/*
    убираем из списка ходов короля те ходы, которые попадают под шах
*/
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

/*
    функция, выдающая положение короля цвета color
*/
int king_position(char cur_board[8][8][2], char color)
{
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (cur_board[i][j][0]=='k' && cur_board[i][j][1]==color)
                return 10*j+i;
        }
    }
    return -1;
}

/*
    здесь записывается ход в массив
*/
void write_turn(char cur_board[8][8][2], int a, int b)
{
    int a2 = a/10, a1 = a%10;
    int b2 = b/10, b1 = b%10;
    cur_board[b1][b2][0] = cur_board[a1][a2][0];
    cur_board[a1][a2][0] = '0';
    cur_board[b1][b2][1] = cur_board[a1][a2][1];
    cur_board[a1][a2][1] = '0';
}

/*
    функция добавления хода на доску
    0 - ход не записан
    1 - ход записан
*/
int add_turn(char cur_board[8][8][2], int a, int b, char cur_color)
{
    if (a<0 || b<0)
        return 0;
    int a1 = a/10, a2 = a%10;
    int b1 = b/10, b2 = b%10;
    int x = b1 - a1;
    if (a1 < 0 || a1 > 7 || a2 < 0 || a2 > 7 || b1 < 0 || b1 > 7 || b2 < 0 || b2 > 7)
        return 0;
    if (!is_on_board(cur_board, a1, a2)) {
        return 0;
    }
    int turn = check_turn(cur_board, a1, a2, b1, b2);
    char color = cur_board[a2][a1][1];
    if (cur_color != color || turn==0) {
        return 0;
    }
    if (cur_board[a2][a1][0] == 'k' && turn != 3) {
        char f1 = cur_board[b2][b1][0], f2 = cur_board[b2][b1][1];
        write_turn(cur_board, a, b);
        if (check_checkmate(cur_board, b1, b2, color)){
            write_turn(cur_board, b, a);
            cur_board[b2][b1][0] = f1;
            cur_board[b2][b1][1] = f2;
            return 0;
        } else {
            return 1;
        }
    }

    int kp = king_position(cur_board, color);
    char f1 = cur_board[b2][b1][0];
    char f2 = cur_board[b2][b1][1];
    write_turn(cur_board, a, b);
    if (check_checkmate(cur_board, kp/10, kp%10, color)){
        write_turn(cur_board, b, a);
        cur_board[b2][b1][0] = f1;
        cur_board[b2][b1][1] = f2;
        return 0;
    }
        write_turn(cur_board, b, a);
        cur_board[b2][b1][0] = f1;
        cur_board[b2][b1][1] = f2;

    if (turn==1){
        write_turn(cur_board, a, b);
        return 1;
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
            return 1;
        } else {
            return 0;
        }
    }
    return 0;
}

/*
    печать доски в консоль
*/
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

/*
    перевод хода из символьной записи в числовую
*/
int from_str(char a[2])
{
    return ((int)a[0]-97)*10 + ((int)a[1]-48-1);
}

/*
    метод копирования доски
*/
void copy_board(const char board1[8][8][2], char board2[8][8][2])
{
    for (int i = 0; i<8; ++i)
        for (int j = 0; j<8; ++j)
            for (int k = 0; k<2; ++k) board2[i][j][k] = board1[i][j][k];
}

/*
    перевод хода из числовой записи в символьную
*/
char* to_str(int a)
{
    char *b = malloc(2);
    b[0] = (char)(a/10+97);
    b[1] = (char)(a%10+48+1);
    return b;
}

/*
    функция, находящая позицию крепости при рокировке
*/
int find_rock(char board[8][8][2], char color, int x)
{
    if (color=='w' && x>0) return 5070;
    if (color=='w' && x<0) return 3000;
    if (color=='b' && x>0) return 5777;
    if (color=='b' && x<0) return 3707;
}

/*
    список, содержащий всевозможные ходы для фигуры, стоящей на позиции а1
*/
vector_int_t turns(char cur_board[8][8][2], int a1)
{
    vector_int_t v;
    v_init(&v);
    for (int i = 0; i<8; ++i)
        for (int j = 0; j<8; ++j){
            char f11 = cur_board[i][j][0], f21 = cur_board[a1%10][a1/10][0];
            char f12 = cur_board[i][j][1], f22 = cur_board[a1%10][a1/10][1];
            int t = check_turn(cur_board, a1/10, a1%10, j, i);
            if (add_turn(cur_board, a1, j*10+i, cur_board[a1%10][a1/10][1])){
                cur_board[i][j][0] = f11;
                cur_board[i][j][1] = f12;
                cur_board[a1%10][a1/10][0] = f21;
                cur_board[a1%10][a1/10][1] = f22;
                v_push(&v, j*10+i);
                if (t==3){
                    int r = find_rock(cur_board, cur_board[a1%10][a1/10][1], j-a1/10);
                    int r1 = r/100, r2 = r%100;
                    cur_board[r1%10][r1/10][0] = '0';
                    cur_board[r1%10][r1/10][1] = '0';
                    cur_board[r2%10][r2/10][0] = 'r';
                    cur_board[r2%10][r2/10][1] = cur_board[a1%10][a1/10][1];
                }
            }
        }
    return v;
}

/*
    метод печати списка ходов фигуры на доске в консоль
*/
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

/*
    функция, проверяющая проигрыш
*/
int loss_q(char cur_board[8][8][2], char color)
{
    char c_board[8][8][2];
    copy_board(cur_board, c_board);
    for (int i = 0; i<8; ++i)
        for (int j = 0; j<8; ++j)
            if (cur_board[i][j][1]==color)
                for (int i1 = 0; i1<8; ++i1)
                    for (int j1 = 0; j1<8; ++j1)
                        if (add_turn(c_board, 10*j+i, 10*j1+i1, color))
                            return 0;

    return 1;
}
