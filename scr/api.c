#include <stdio.h>
#include <windows.h>

LRESULT WINAPI DefWindProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
    if (message == WM_DESTROY) 
        PostQuitMessage(0);
    else if (message == WM_CHAR) printf("%c\n", wparam);
    else if (message == WM_MOUSEMOVE) {
        int x = LOWORD(lparam);
        int y = HIWORD(lparam);
        printf("[%d, %d]", x, y);
    }
    else if (message == WM_LBUTTONDOWN) printf("I click!");
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
    w.hbrBackground = (HBRUSH)(DKGRAY_BRUSH);
    RegisterClassA(&w);

    HWND hwnd;
    hwnd = CreateWindow("Window", "Hello, it's my window!", WS_OVERLAPPEDWINDOW, 100, 100, 640, 480, NULL, NULL, hInstance, NULL);
    ShowWindow(hwnd, SW_NORMAL);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}