#include <stdio.h>
#include <windows.h>
#define win_len 1000
#define win_height 700

HWND bt_color;
HWND bt_again;
HWND label;

char color = 'w';

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

void game_restart()
{
    copy_board(new_board, board);
}

int from_win_coord(int x, int y)
{
    return 10*x+y;
}

void draw_turn(int player_turn)
{
    int a = player_turn/10;
    int b = player_turn%10;
    printf("%d %d %d %d\n", a/10, a%10, b/10, b%10);
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
        } else {
            first_click = 0;
            player_turn = 100*player_turn+from_win_coord(x, y);
            draw_turn(player_turn);
        }
    }
    else if (message == WM_COMMAND){
        printf("WM_COMMAND\n");
        if (bt_color == lparam){
            color = (color=='w'? 'b':'w');
            SetWindowText(label, color=='w'? "You are playing white.":"You are playing black.");
            game_restart();
        };
        if (bt_again == lparam) game_restart();
    }
    else return DefWindowProcA(hwnd, message, wparam, lparam);
}

int WINAPI WinMain(HINSTANCE hInstance, // дескриптор (идентификатор программы)
                    HINSTANCE hPrevInstance, // предыдущий дескриптор процесса, использовался в ранних версиях Windows, сейчас устарел
                    LPSTR lpCmdLine, // аргументы командной строки в виде строки Юникода
                    int CmdShow) // управление способом отображения окна
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

    game_restart();

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}
