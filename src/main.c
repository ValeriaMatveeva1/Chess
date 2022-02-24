#include "main.h"

HWND bt_color;
HWND bt_again;
HWND label, label2;

HINSTANCE hInst;

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
char color = 'w';

void game_restart()
{
    copy_board(new_board, board);
}

int from_win_coord(int x, int y)
{
    for (int i = 0; i<8; ++i){
        for (int j = 0; j<8; ++j){
                int x1 = 10+(box_len+1)*i;
                int y1 = 10+(box_len+1)*j;
                if ((x>x1) && (x<x1+(box_len+1)) && (y>y1) && (y<y1+(box_len+1)))
                    return color=='w'? i*10+7-j : (7-i)*10+j;
        }
    }
    return -1;
}

void draw_turn(int player_turn, HWND hwnd)
{
    int a = player_turn/100, b = player_turn%100;
    if (player_turn==-100-1) return;
    if (add_turn(board, player_turn/100, player_turn%100, color)) {
        SetWindowText(label2, "Bot turn.");
        draw_bitmaps(hwnd);
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
    if (loss_q(board, color))
        SetWindowText(label, "You lose.");
    if (loss_q(board, color=='w'?'b':'w'))
        SetWindowText(label, "You win.");
    draw_bitmaps(hwnd);
}

char *get_path(char a1, char a2, int flag)
{
    char *way = "src\\assets\\";
    char *res = malloc(19);
    for (int i = 0; i<11; ++i){
        res[i] = way[i];
    }
    res[11] = a1;
    res[12] = a2;
    res[13] = (char)(flag+48);
    res[14] = '.';
    res[15] = 'b';
    res[16] = 'm';
    res[17] = 'p';
    res[18] = '\0';
    return res;
}

void draw_bitmaps(HWND hwnd)
{
    InvalidateRect(hwnd, NULL, TRUE);
    PAINTSTRUCT ps;
    HDC hPaintDC = BeginPaint(hwnd, &ps);
    HDC hMemDC = CreateCompatibleDC(hPaintDC);
    HGDIOBJ hOld;
    BITMAP bmp; HBITMAP hbmp;

    for (int i = 0; i<8; ++i)
        for (int j = 0; j<8; ++j){
            int i1 = color=='w'?7-i:i, j1 = color=='w'?j:7-j, temp, t = j1*10+i1;
            v_find(&possible_turns, j1*10+i1, temp);

            int c = (board[player_turn%10][player_turn/10][1]==color && temp!=-1 ||
                     (first_click==1 && (t==player_turn)) ? 2 :
                     (enemy_turn!=-1 && (enemy_turn%100==t ||
                                         enemy_turn/100==t)) ? 3 : (i%2+j%2)%2);
            char *name = get_path(board[i1][j1][0], board[i1][j1][1], c);
            hbmp = (HBITMAP)LoadImage(hInst,
                                      name,
                                      IMAGE_BITMAP,
                                      0, 0,
                                      LR_LOADFROMFILE
                                      );
            GetObject(hbmp, sizeof (BITMAP), &bmp);
            hOld = SelectObject(hMemDC, hbmp);
            StretchBlt(hPaintDC,
                       10+(box_len+1)*j, 10+(box_len+1)*i,
                       box_len, box_len, hMemDC,
                       0, 0,
                       bmp.bmWidth, bmp.bmHeight,
                       SRCCOPY);
            SelectObject (hMemDC, hOld);
        }
    DeleteDC (hMemDC);
    EndPaint(hwnd, &ps);
}

LRESULT WINAPI DefWindProc(HWND hwnd,
                           UINT message,
                           WPARAM wparam,
                           LPARAM lparam
                           )
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
            draw_bitmaps(hwnd);
        } else {
            v_init(&possible_turns);
            first_click = 0;
            player_turn = 100*player_turn+from_win_coord(x, y);
            draw_turn(player_turn, hwnd);
        }
    }
    else if (message == WM_COMMAND){
        if (bt_color == lparam){
            enemy_turn = -1;
            v_clear(&possible_turns);
            color = (color=='w'? 'b':'w');
            SetWindowText(label,
                          color=='w'?
                          "You are playing white.":"You are playing black."
                          );
            game_restart();
            if (color=='b'){
                int bt = minimax(board, 'w', 2);
                enemy_turn = bt;
                write_turn(board, bt/100, bt%100);
            }
            draw_bitmaps(hwnd);
        }
        if (bt_again == lparam) {
            v_clear(&possible_turns);
            enemy_turn = -1;
            SetWindowText(label,
                          color=='w'?
                          "You are playing white.":"You are playing black."
                          );
            game_restart();
            if (color=='b'){
                int bt = minimax(board, 'w', 2);
                enemy_turn = bt;
                write_turn(board, bt/100, bt%100);
            }
            draw_bitmaps(hwnd);
        }
    } else if (message == WM_PAINT) {
        draw_bitmaps(hwnd);
    }
    else return DefWindowProcA(hwnd, message, wparam, lparam);
}

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int CmdShow)
{

    WNDCLASSA w;
    memset(&w, 0, sizeof(WNDCLASSA));
    w.hInstance = hInstance;
    hInst = hInstance;
    w.lpszClassName = "Window";
    w.lpfnWndProc = DefWindProc;
    RegisterClassA(&w);

    HWND hwnd;
    hwnd = CreateWindow("Window",
                        "Chess",
                        WS_OVERLAPPEDWINDOW,
                        200, 100,
                        win_len, win_height,
                        NULL, NULL, hInstance, NULL
                        );
    ShowWindow(hwnd, SW_NORMAL);
    /*UpdateWindow(hwnd);*/

    bt_color = CreateWindow("button",
                            "Change color",
                            WS_VISIBLE | WS_CHILD,
                            win_len-200, win_height/2-25,
                            150, 50,
                            hwnd,
                            NULL, NULL, NULL
                            );
    bt_again = CreateWindow("button",
                            "Play again",
                            WS_VISIBLE | WS_CHILD,
                            win_len-200, win_height/2+30,
                            150, 50,
                            hwnd,
                            NULL, NULL, NULL
                            );

    label = CreateWindow("static",
                         "You are playing white.",
                         WS_VISIBLE | WS_CHILD,
                         win_len-200, win_height/2+85,
                         150, 50,
                         hwnd,
                         NULL, NULL, NULL
                         );
    label2 = CreateWindow("static",
                          "Your turn.",
                          WS_VISIBLE | WS_CHILD,
                          win_len-200, win_height/2+140,
                          150, 50,
                          hwnd,
                          NULL, NULL, NULL
                          );

    v_init(&possible_turns);
    game_restart();

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}
