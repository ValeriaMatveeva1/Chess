#include <stdio.h>
#include <windows.h>
#define win_len 1000
#define win_height 700
#define box_len 80


HWND bt_color;
HWND bt_again;
HWND label;

HWND labels[8][8];

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
    re_draw_board();
}

int from_win_coord(int x, int y)
{
    for (int i = 0; i<8; ++i){
        for (int j = 0; j<8; ++j){
                int x1 = 10+(box_len+1)*i;
                int y1 = 10+(box_len+1)*j;
                if ((x>x1) && (x<x1+(box_len+1)) && (y>y1) && (y<y1+(box_len+1))) return i*10+7-j;
        }
    }
    return -1;
}

void re_draw_board()
{
    for (int i = 0; i<8; ++i){
        for (int j = 0; j<8; ++j){
            char temp[3]; temp[0] = board[7-i][j][0]; temp[1] = board[7-i][j][1]; temp[2] = '\0';
            SetWindowText(labels[i][j], temp[0]!='0'?temp:'\0');
        }
    }
}

void draw_turn(int player_turn)
{
    int a = player_turn/100, b = player_turn%100;
    if (player_turn==-1) return;
    char *rt1 = to_str(a); char* rt2 = to_str(b);
    printf("%c%c %c%c\n", rt1[0], rt1[1], rt2[0], rt2[1]);
    if (add_turn(board, player_turn/100, player_turn%100, color)) {
        int bt = minimax(board, color=='w'?'b':'w', 2);
        write_turn(board, bt/100, bt%100);
    }

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
            re_draw_board();
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
            if (color=='b'){
                int bt = minimax(board, 'w', 2);
                write_turn(board, bt/100, bt%100);
                re_draw_board();
            }
        };
        if (bt_again == lparam) {
            game_restart();
            if (color=='b'){
                int bt = minimax(board, 'w', 2);
                write_turn(board, bt/100, bt%100);
                re_draw_board();
            }
        }
    } else if (message==WM_CTLCOLORSTATIC) {
        if (first_click && lparam==labels[7-player_turn%10][player_turn/10]){
        SetBkMode((HDC)wparam, TRANSPARENT);
        return CreateSolidBrush(RGB(201,201,201));
        } else {
            int temp = 0;
            for (int i = 0; i<8; ++i)
                for (int j = 0; j<8; ++j) if ((i%2+j%2)%2 && lparam==labels[i][j]) {temp = 1; break;};
            if (temp){
                SetBkMode((HDC)wparam, TRANSPARENT);
                return CreateSolidBrush(RGB(220,220,220));
            }
        }
    }
    else return DefWindowProcA(hwnd, message, wparam, lparam);
}

int WINAPI WinMain(HINSTANCE hInstance, // äåñêðèïòîð (èäåíòèôèêàòîð ïðîãðàììû)
                    HINSTANCE hPrevInstance, // ïðåäûäóùèé äåñêðèïòîð ïðîöåññà, èñïîëüçîâàëñÿ â ðàííèõ âåðñèÿõ Windows, ñåé÷àñ óñòàðåë
                    LPSTR lpCmdLine, // àðãóìåíòû êîìàíäíîé ñòðîêè â âèäå ñòðîêè Þíèêîäà
                    int CmdShow) // óïðàâëåíèå ñïîñîáîì îòîáðàæåíèÿ îêíà
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

    for (int i = 0; i<8; ++i){
        for (int j = 0; j<8; ++j){
            char temp[3]; temp[0] = board[7-i][j][0]; temp[1] = board[7-i][j][1]; temp[2] = '\0';
            labels[i][j] = CreateWindow("static", temp[0]!='0'?temp:'\0', WS_VISIBLE | WS_CHILD| SS_CENTER| WS_EX_CLIENTEDGE, 10+(box_len+1)*j, 10+(box_len+1)*i, box_len, box_len, hwnd, NULL, NULL, NULL);
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
