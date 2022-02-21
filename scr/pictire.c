#include <stdio.h>
#include <windows.h>
#include <stdlib.h>

#define FILE_MENU_NEW 1
#define FILE_MENU_OPEN 2
#define FILE_MENU_EXIT 3
#define GENERATE_BUTTON 4

LRESULT WINAPI WindowProcedure(HWND, UINT, WPARAM, LPARAM);

void AddMenus(HWND);
void AddControls(HWND);
void loadImages();

HWND hName, hAge, hOut, hLogo;
HMENU hMenu;
HBITMAP hLogoImage, hGenerateImage;

int WINAPI WinMain(HINSTANCE hInstance, // дескриптор (идентификатор программы)
                    HINSTANCE hPrevInstance, // предыдущий дескриптор процесса, использовался в ранних версиях Windows, сейчас устарел
                    LPSTR lpCmdLine, // аргументы командной строки в виде строки Юникода
                    int CmdShow) // управление способом отображения окна
{
    WNDCLASSW w = {0};
    
    w.hbrBackground = (HBRUSH)COLOR_WINDOW;
    w.hCursor = LoadCursor(NULL, IDC_ARROW);
    w.hInstance = hInstance;
    w.lpszClassName = L"Window";
    w.lpfnWndProc = WindowProcedure;
    
    if (!RegisterClassW(&w))
      return -1;

    CreateWindowW(L"Window", L"Hello, it's my window!", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 500, 500, NULL, NULL, NULL, NULL);

    MSG msg = {0};

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}

LRESULT WINAPI WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg)
    {
    case WM_COMMAND:
      switch (wparam)
      {
      case FILE_MENU_EXIT:
        DestroyWindow(hwnd);
        break;
      case FILE_MENU_NEW:
        MessageBeep(MB_ICONINFORMATION);
        break;
      case GENERATE_BUTTON: ;
        char name[30], age[10], out[50];
        GetWindowText(hName, name, 30);
        GetWindowText(hAge, age, 10);

        strcpy(out, name);
        strcat(out, " is ");
        strcat(out, age);
        strcat(out, " years old.");

        SetWindowText(hOut, out);
        break;
    }
      break;
    case WM_CREATE:
      AddMenus(hwnd);
      AddControls(hwnd);
      break;
    case WM_DESTROY:
      PostQuitMessage(0);
      break;
    default:
      return DefWindowProcW(hwnd, msg, wparam, lparam);
    }
}

void AddMenus(HWND hwnd)
{
  hMenu = CreateMenu();
  HMENU hFileMenu = CreateMenu();
  HMENU hSubMenu = CreateMenu();

  AppendMenu(hFileMenu,MF_STRING, FILE_MENU_NEW,"New");
  AppendMenu(hFileMenu,MF_POPUP, (UINT_PTR)hSubMenu,"Open SubMenu");
  AppendMenu(hFileMenu,MF_STRING, FILE_MENU_EXIT,"Exit");

  AppendMenu(hMenu,MF_POPUP, (UINT_PTR)hFileMenu,"FIle");

  SetMenu(hwnd, hMenu);
}

void AddControls(HWND hwnd) 
{
  CreateWindowW(L"Static", L"Name: ", WS_VISIBLE | WS_CHILD, 100, 50, 98, 38, hwnd, NULL, NULL, NULL);
  hName = CreateWindowW(L"Edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 200, 50, 98, 38, hwnd, NULL, NULL, NULL);

  CreateWindowW(L"Static", L"Age: ", WS_VISIBLE | WS_CHILD, 100, 90, 98, 38, hwnd, NULL, NULL, NULL);
  hAge = CreateWindowW(L"Edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 200, 90, 98, 38, hwnd, NULL, NULL, NULL);

  CreateWindowW(L"Button", L"Generate", WS_VISIBLE | WS_CHILD, 150, 140, 98, 38, hwnd, (HMENU)GENERATE_BUTTON, NULL, NULL);
  hOut = CreateWindowW(L"Edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 100, 200, 300, 200, hwnd, NULL, NULL, NULL);

  hLogo = CreateWindowW(L"Static", NULL, WS_VISIBLE | WS_CHILD | SS_BITMAP, 100, 60, 100, 100, hwnd, NULL, NULL, NULL);
  SendMessageW(hLogo, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hLogoImage);
}

void loadImages()
{
  
  hLogoImage = (HBITMAP)LoadImageW(NULL, "2.bmp", IMAGE_BITMAP,0, 0, LR_LOADFROMFILE);
}




// void DrawBitmap(HDC hDC, int x, int y, HBITMAP hBitmap)
// {
//   HBITMAP hbm, hOldbm;
//   HDC hMemDC;
//   BITMAP bm;
//   POINT  ptSize, ptOrg;

  
//   hMemDC = CreateCompatibleDC(hDC);

//   hOldbm = (HBITMAP)SelectObject(hMemDC, hBitmap);

//   if (hOldbm)
//   {
    
//     SetMapMode(hMemDC, GetMapMode(hDC));

//     GetObject(hBitmap, sizeof(BITMAP), (LPSTR)&bm);

//     ptSize.x = bm.bmWidth;   
//     ptSize.y = bm.bmHeight; 

//     DPtoLP(hDC, &ptSize, 1);

//     ptOrg.x = 0;
//     ptOrg.y = 0;

//     DPtoLP(hMemDC, &ptOrg, 1);

//     BitBlt(hDC, x, y, ptSize.x, ptSize.y,
//            hMemDC, ptOrg.x, ptOrg.y, SRCCOPY);

//     SelectObject(hMemDC, hOldbm);
//   }

//   DeleteDC(hMemDC);
// }