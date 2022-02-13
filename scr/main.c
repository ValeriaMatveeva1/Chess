#include "main.h"

int main()
{
    char board[8][8][2] = {
        {{'r','w'},{'n','w'},{'b','w'},{'q','w'},{'k','w'},{'b','w'},{'n','w'},{'r','w'}},
        {{'p','w'},{'p','w'},{'p','w'},{'p','w'},{'p','w'},{'p','w'},{'p','w'},{'p','w'}},
        {{'0','0'},{'0','0'},{'0','0'},{'0','0'},{'0','0'},{'0','0'},{'0','0'},{'0','0'}},
        {{'0','0'},{'0','0'},{'0','0'},{'0','0'},{'0','0'},{'0','0'},{'0','0'},{'0','0'}},
        {{'0','0'},{'0','0'},{'0','0'},{'0','0'},{'0','0'},{'0','0'},{'0','0'},{'0','0'}},
        {{'0','0'},{'0','0'},{'0','0'},{'0','0'},{'0','0'},{'0','0'},{'0','0'},{'0','0'}},
        {{'p','b'},{'p','b'},{'p','b'},{'p','b'},{'p','b'},{'p','b'},{'p','b'},{'p','b'}},
        {{'r','b'},{'n','b'},{'b','b'},{'q','b'},{'k','b'},{'b','b'},{'n','b'},{'r','b'}}
    };
    add_turn(board, from_str("e2"), from_str("e4"), 'w');
    add_turn(board, from_str("e7"), from_str("e5"), 'b');
    add_turn(board, from_str("b1"), from_str("c3"), 'w');
    add_turn(board, from_str("f8"), from_str("c5"), 'b');
    add_turn(board, from_str("c3"), from_str("b5"), 'w');
    add_turn(board, from_str("g8"), from_str("h6"), 'b');
    add_turn(board, from_str("g2"), from_str("g4"), 'w');
    add_turn(board, from_str("g7"), from_str("g6"), 'b');
    add_turn(board, from_str("g1"), from_str("f3"), 'w');
    add_turn(board, from_str("c5"), from_str("b6"), 'b');
    add_turn(board, from_str("g4"), from_str("g5"), 'w');
    add_turn(board, from_str("d7"), from_str("d5"), 'b');
    add_turn(board, from_str("e4"), from_str("d5"), 'w');
    add_turn(board, from_str("h6"), from_str("g4"), 'b');
    add_turn(board, from_str("b5"), from_str("c7"), 'w');
    add_turn(board, from_str("e8"), from_str("d7"), 'b');
    add_turn(board, from_str("c7"), from_str("a8"), 'w');
    add_turn(board, from_str("d8"), from_str("f8"), 'b');
    add_turn(board, from_str("a8"), from_str("b6"), 'w');
    add_turn(board, from_str("d7"), from_str("c7"), 'b');
    add_turn(board, from_str("b6"), from_str("d8"), 'w');
    add_turn(board, from_str("f8"), from_str("c5"), 'b');
    add_turn(board, from_str("f1"), from_str("h3"), 'w');
    add_turn(board, from_str("c5"), from_str("f2"), 'b');
    
    print_turns(board, turns(board, from_str("e1")));
    printf("%d\n", loss_q(board, 'w'));
    // print_board(board);
    // for (int i = 0; i<8; ++i)
    //     for (int j = 0; j<8; ++j)
    //         if (board[j][i][0]!='0'){
    //         printf("%c%c \n", to_str(i*10+j)[0], to_str(i*10+j)[1]);
    //         vector_int_t v = turns(board, i*10+j);
    //         print_turns(board, v);
    //         v_clear(&v);
    //         getchar();
    //         }
        

    return 0;
}