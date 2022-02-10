#include "../vector.c"
#include <stdio.h>
#include <stdlib.h>
#define or ||
#define and &&

int* to_tuple(char x, char y) {
     int* int_tuple = (int*) calloc(2, sizeof(int));
     int_tuple[0] = (int)x - 97;
     int_tuple[1] = y - 1;
     return int_tuple;
}

char* from_tuple(int x, int y) {
    char* char_tuple = (char*) calloc(2, sizeof(char));
    char_tuple[0] = (char)(x + 97);
    char_tuple[1] = (char)(y + 48);
    return char_tuple;
}

int is_on_board(char cur_board[8][8][2], int x, int y) {
    return cur_board[x][y][0] != '0';
}

int check_turn(char cur_board[8][8][2], int a1, int a2, int b1, int b2) {
    return 0;
}

int check_checkmate(char cur_board[8][8][2], int x, int y, char color) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (is_on_board(cur_board, i, j) and cur_board[i][j][1] != color and check_turn(cur_board, i, j, x, y)) {
                return 1;
            }
        }
    }
    return 0;
}

vector_int_t get_king_turns(int x, int y) {
    vector_int_t king_turns;
    if (x > 0 and y > 0) v_push(&king_turns, (x - 1)*10 + (y - 1));
    if (x > 0 and y < 7) v_push(&king_turns, (x - 1)*10 + (y + 1));
    if (x < 7 and y > 0) v_push(&king_turns, (x + 1)*10 + (y - 1));
    if (x < 7 and y < 7) v_push(&king_turns, (x + 1)*10 + (y + 1));
    if (x > 0)v_push(&king_turns, (x - 1)*10 + y);
    if (x < 7)v_push(&king_turns, (x + 1)*10 + y);
    if (y > 0) v_push(&king_turns, x*10 + (y - 1));
    if (y < 7) v_push(&king_turns, x*10 + (y + 1));
    return king_turns;
}

void delete_checkmates_for_king(char cur_board[8][8][2], vector_int_t king_turns, char color) {
    int c = 0;
    while (c != king_turns.len) {
        if (check_checkmate(cur_board, king_turns.data[c]/10, king_turns.data[c]%10, color)) {
            v_pop(&king_turns, c);
        } else {
            c++;
        }
    }
}

int add_turn(char cur_board[8][8][2], char* a, char* b, char cur_color) {
    int* res_a = to_tuple(a[0], a[1]);
    int a1 = res_a[0], a2 = res_a[1];
    int* res_b = to_tuple(b[0], b[1]);
    int b1 = res_b[0], b2 = res_b[1];
    int x = b1 - a1, y = b2 - a2;
    if (a1 < 0 or a1 > 7 or a2 < 0 or a2 > 7 or b1 < 0 or b1 > 7 or b2 < 0 or b2 > 7) {
    return -1;
    }
    if (!is_on_board(cur_board, a1, a2)) {
        return 0;
    }
    int turn = check_turn(cur_board, a1, a2, b1, b2);
    char color = cur_board[a1][a2][1];
    if (cur_color != color) {
        return -1;
    }
    if (cur_board[a1][a2][0] == 'k' and turn != 3) {
        vector_int_t all_king_turns = get_king_turns(a1, a2);
        delete_checkmates_for_king(cur_board, all_king_turns, color);
        if (v_find(&all_king_turns, b1*10+b2)) {
            cur_board[b1][b2][0] = cur_board[a1][a2][0];
            cur_board[a1][a2][0] = '0';
            cur_board[b1][b2][1] = cur_board[a1][a2][1];
            cur_board[a1][a2][1] = '0';
        }
    }
}


int main()
{
    char board[8][8][2] = {
        {"rw","nw","bw","qw","kw","bw","nw","rw"},
        {"pw","pw","pw","pw","pw","pw","pw","pw"},
        {"00","00","00","00","00","00","00","00"},
        {"00","00","00","00","00","00","00","00"},
        {"00","00","00","00","00","00","00","00"},
        {"00","00","00","00","00","00","00","00"},
        {"pb","pb","pb","pb","pb","pb","pb","pb"},
        {"rb","nb","bb","qb","kb","bb","nb","rb"}

    };
    // vector_int_t v;
    // v_init(&v);
    // v_pop(&v, 1);
    // v_insert(&v, 10, 24);
    // for (int i = 0; i<5; ++i) v_push(&v,i);
    // v_pop(&v, 3);
    // v_insert(&v, 1, 24);
    // for (int i = 0; i<v.len; ++i) printf("%d\n", v.data[i]);

    return 0;
}