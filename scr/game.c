#include "game.h"

const char start_board[8][8][2] = {
        {{'r','w'},{'n','w'},{'b','w'},{'q','w'},{'k','w'},{'b','w'},{'n','w'},{'r','w'}},
        {{'p','w'},{'p','w'},{'p','w'},{'p','w'},{'p','w'},{'p','w'},{'p','w'},{'p','w'}},
        {{'0','0'},{'0','0'},{'0','0'},{'0','0'},{'0','0'},{'0','0'},{'0','0'},{'0','0'}},
        {{'0','0'},{'0','0'},{'0','0'},{'0','0'},{'0','0'},{'0','0'},{'0','0'},{'0','0'}},
        {{'0','0'},{'0','0'},{'0','0'},{'0','0'},{'0','0'},{'0','0'},{'0','0'},{'0','0'}},
        {{'0','0'},{'0','0'},{'0','0'},{'0','0'},{'0','0'},{'0','0'},{'0','0'},{'0','0'}},
        {{'p','b'},{'p','b'},{'p','b'},{'p','b'},{'p','b'},{'p','b'},{'p','b'},{'p','b'}},
        {{'r','b'},{'n','b'},{'b','b'},{'q','b'},{'k','b'},{'b','b'},{'n','b'},{'r','b'}}
};

char ask_color()
{
    printf("Enter the color: ");
    char c;
    int temp = scanf("%c", &c);
    if (temp==1 && (c=='w' || c=='b'))
        return c;
    else printf("Invalid input.\n");
    return '\n';
}

int ask_turn()
{
    printf("Enter the turn: ");
    char a[2], b[2], t[10];
    scanf("%s", t);
    a[0] = t[0]; a[1] = t[1]; b[0] = t[2]; b[1] = t[3];
    int res = 100*from_str(a)+from_str(b);
    return 100*from_str(a)+from_str(b);
}

void write_bot_turn(char cur_board[8][8][2], char color)
{
    int bt = minimax(cur_board, color, 2);
    write_turn(cur_board, bt/100, bt%100);
}

void game(char color)
{
    char flag = color;
    char board[8][8][2];
    copy_board(start_board, board);
    if (color=='b') write_bot_turn(board, 'w');
    while(1){
        printf("%s turn\n", flag=='w'?"White":"Black");
        if (flag==color){
            print_board(board);
            int t = ask_turn();
            //system("cls");
            if (add_turn(board, t/100, t%100, flag)) flag = (flag=='b' ? 'w':'b');
        } else {
            //system("cls");
            write_bot_turn(board, flag);
            flag = (flag=='b' ? 'w':'b');
        }
    }
}
