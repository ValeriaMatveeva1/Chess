#include "main.h"

HWND bt_color;
HWND bt_again;
HWND label, label2;

HWND labels[8][8];

char color = 'w';

vector_int_t possible_turns;


const char new_board[8][8][2] = {
        {{'r','w'},{'n','w'},{'b','w'},{'q','w'},{'k','w'},{'b','w'},{'n','w'},{'r','w'}},
        {{'p','w'},{'p','w'},{'p','w'},{'p','w'},{'p','w'},{'p','w'},{'p','w'},{'p','w'}},
        {{'0','0'},{'0','0'},{'0','0'},{'0','0'},{'0','0'},{'0','0'},{'0','0'},{'0','0'}},
        {{'0','0'},{'0','0'},{'0','0'},{'0','0'},{'0','0'},{'0','0'},{'0','0'},{'0','0'}},
        {{'0','0'},{'0','0'},{'0','0'},{'0','0'},{'0','0'},{'0','0'},{'0','0'},{'0','0'}},
        {{'0','0'},{'0','0'},{'0','0'},{'0','0'},{'0','0'},{'0','0'},{'0','0'},{'0','0'}},
        {{'p','b'},{'p','b'},{'p','b'},{'p','b'},{'p','b'},{'p','b'},{'p','b'},{'p','b'}},
        {{'r','b'},{'n','b'},{'b','b'},{'q','b'},{'k','b'},{'b','b'},{'n','b'},{'r','b'}}
};

char board[8][8][2];
int first_click = 0;
int player_turn;
int enemy_turn = -1;

void game_restart()
{
    copy_board(new_board, board);
    re_draw_board();
}

int from_win_coord(int x, int y)
{
    for (int i = 0; i<8; ++i){
        for (int j = 0; j<8; ++j){
                int x1 = 10+(box_len+1)*i;
                int y1 = 10+(box_len+1)*j;
                if ((x>x1) && (x<x1+(box_len+1)) && (y>y1) && (y<y1+(box_len+1))) return color=='w'? i*10+7-j : (7-i)*10+j;
        }
    }
    return -1;
}

char* long_fig_name(char short_name[2])
{
    if (short_name[1]=='w'){
        if (short_name[0]=='p') return "\nwhite\npawn\0";
        if (short_name[0]=='n') return "\nwhite\nknight\0";
        if (short_name[0]=='b') return "\nwhite\nbishop\0";
        if (short_name[0]=='q') return "\nwhite\nqueen\0";
        if (short_name[0]=='k') return "\nwhite\nking\0";
        if (short_name[0]=='r') return "\nwhite\nrock\0";
    } else if (short_name[1]=='b') {
        if (short_name[0]=='p') return "\nblack\npawn\0";
        if (short_name[0]=='n') return "\nblack\nknight\0";
        if (short_name[0]=='b') return "\nblack\nbishop\0";
        if (short_name[0]=='q') return "\nblack\nqueen\0";
        if (short_name[0]=='k') return "\nblack\nking\0";
        if (short_name[0]=='r') return "\nblack\nrock\0";
    } else {
        return "\0";
    }

}

void re_draw_board()
{
    for (int i = 0; i<8; ++i){
        for (int j = 0; j<8; ++j){
            if (color=='w'){
                SetWindowText(labels[i][j], long_fig_name(board[7-i][j]));
            } else {
                SetWindowText(labels[i][j], long_fig_name(board[i][7-j]));
            }

        }
    }
}

void draw_turn(int player_turn)
{
    int a = player_turn/100, b = player_turn%100;
    if (player_turn==-100-1) return;
    char *rt1 = to_str(a); char* rt2 = to_str(b);
    /*printf("%c%c %c%c\n", rt1[0], rt1[1], rt2[0], rt2[1]);*/
    if (add_turn(board, player_turn/100, player_turn%100, color)) {
        SetWindowText(label2, "Bot turn.");
        re_draw_board();
        int bt = minimax(board, color=='w'?'b':'w', 4);
        enemy_turn = bt;
        write_turn(board, bt/100, bt%100);
        int kp = king_position(board, color);
        if (check_checkmate(board, kp/10, kp%10, color)){
            SetWindowText(label2, "Check.");
        } else {
            SetWindowText(label2, "Your turn.");
        }
    }
    if (loss_q(board, color)) SetWindowText(label, "You lose.");
    if (loss_q(board, color=='w'?'b':'w')) SetWindowText(label, "You win.");
    re_draw_board();
}

LRESULT WINAPI DefWindProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
    if (message == WM_DESTROY)
        PostQuitMessage(0);
    else if (message == WM_LBUTTONDOWN){
        int x = LOWORD(lparam);
        int y = HIWORD(lparam);
        if (first_click==0){
            player_turn = from_win_coord(x, y);
            first_click = 1;
            possible_turns = turns(board, player_turn);
            re_draw_board();
        } else {
            first_click = 0;
            player_turn = 100*player_turn+from_win_coord(x, y);
            draw_turn(player_turn);
        }
    }
    else if (message == WM_COMMAND){
        if (bt_color == lparam){
            enemy_turn = -1;
            v_clear(&possible_turns);
            color = (color=='w'? 'b':'w');
            SetWindowText(label, color=='w'? "You are playing white.":"You are playing black.");
            game_restart();
            if (color=='b'){
                int bt = minimax(board, 'w', 2);
                enemy_turn = bt;
                write_turn(board, bt/100, bt%100);
                re_draw_board();
            }
        }
        if (bt_again == lparam) {
            v_clear(&possible_turns);
            enemy_turn = -1;
            SetWindowText(label, color=='w'? "You are playing white.":"You are playing black.");
            game_restart();
            if (color=='b'){
                int bt = minimax(board, 'w', 2);
                enemy_turn = bt;
                write_turn(board, bt/100, bt%100);
                re_draw_board();
            }
        }
    } else if (message==WM_CTLCOLORSTATIC) {
        if (color=='w'){
            if (first_click && lparam==labels[7-player_turn%10][player_turn/10]){
                SetBkMode((HDC)wparam, TRANSPARENT);
                return CreateSolidBrush(RGB(201,201,201));
            }
            int temp = 0;
            for (int i = 0; i<possible_turns.len; ++i)
            if (labels[7-possible_turns.data[i]%10][possible_turns.data[i]/10] == lparam) {temp = 1; break;}
            if (first_click && temp && player_turn!=-1 && board[player_turn%10][player_turn/10][1]==color){
                SetBkMode((HDC)wparam, TRANSPARENT);
                return CreateSolidBrush(RGB(193,255,184));
            }
            if (enemy_turn!=-1 && (lparam==labels[7-(enemy_turn/100)%10][(enemy_turn/100)/10] || lparam==labels[7-(enemy_turn%100)%10][(enemy_turn%100)/10])){
                SetBkMode((HDC)wparam, TRANSPARENT);
                return CreateSolidBrush(RGB(255,185,185));
            }

        } else if (color=='b'){
            if (first_click && lparam==labels[player_turn%10][7-player_turn/10]){
                SetBkMode((HDC)wparam, TRANSPARENT);
                return CreateSolidBrush(RGB(201,201,201));
            }
            int temp = 0;
            for (int i = 0; i<possible_turns.len; ++i)
            if (labels[possible_turns.data[i]%10][7-possible_turns.data[i]/10] == lparam) {temp = 1; break;}
            if (first_click && temp && player_turn!=-1 && board[player_turn%10][player_turn/10][1]==color){
                SetBkMode((HDC)wparam, TRANSPARENT);
                return CreateSolidBrush(RGB(193,255,184));
            }
            if (enemy_turn!=-1 && (lparam==labels[(enemy_turn/100)%10][7-(enemy_turn/100)/10] || lparam==labels[(enemy_turn%100)%10][7-(enemy_turn%100)/10])){
                SetBkMode((HDC)wparam, TRANSPARENT);
                return CreateSolidBrush(RGB(255,185,185));
            }
        }
        int temp = 0;
        for (int i = 0; i<8; ++i)
            for (int j = 0; j<8; ++j) if ((i%2+j%2)%2 && lparam==labels[i][j]) {temp = 1; break;};
        if (temp){
            SetBkMode((HDC)wparam, TRANSPARENT);
            return CreateSolidBrush(RGB(220,220,220));
        }
    }
    else return DefWindowProcA(hwnd, message, wparam, lparam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int CmdShow)
{
    WNDCLASSA w;
    memset(&w, 0, sizeof(WNDCLASSA));
    w.hInstance = hInstance;
    w.lpszClassName = "Window";
    w.lpfnWndProc = DefWindProc;
    RegisterClassA(&w);

    HWND hwnd;
    hwnd = CreateWindow("Window", "Chess", WS_OVERLAPPEDWINDOW, 200, 100, win_len, win_height, NULL, NULL, hInstance, NULL);
    ShowWindow(hwnd, SW_NORMAL);
    UpdateWindow(hwnd);

    bt_color = CreateWindow("button", "Change color", WS_VISIBLE | WS_CHILD, win_len-200, win_height/2-25, 150, 50, hwnd, NULL, NULL, NULL);
    bt_again = CreateWindow("button", "Play again", WS_VISIBLE | WS_CHILD, win_len-200, win_height/2+30, 150, 50, hwnd, NULL, NULL, NULL);


    label = CreateWindow("static", "You are playing white.", WS_VISIBLE | WS_CHILD, win_len-200, win_height/2+85, 150, 50, hwnd, NULL, NULL, NULL);
    label2 = CreateWindow("static", "Your turn.", WS_VISIBLE | WS_CHILD, win_len-200, win_height/2+140, 150, 50, hwnd, NULL, NULL, NULL);

    game_restart();
    v_init(&possible_turns);

    for (int i = 0; i<8; ++i){
        for (int j = 0; j<8; ++j){
            labels[i][j] = CreateWindow("static", long_fig_name(board[7-i][j]), WS_VISIBLE | WS_CHILD| SS_CENTER, 10+(box_len+1)*j, 10+(box_len+1)*i, box_len, box_len, hwnd, NULL, NULL, NULL);
        }
    }


    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}
