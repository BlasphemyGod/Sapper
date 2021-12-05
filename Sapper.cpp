// Sapper.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "Sapper.h"
#include "Field.h"
#include "Cell.h"
#include "Timer.h"
#include "SaveManager.h"
#include "RecordsTable.h"

#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;                                
WCHAR szTitle[MAX_LOADSTRING];                  
WCHAR szWindowClass[MAX_LOADSTRING];            


ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

Field field = new_field(0, 100, 30);
HBRUSH headerBrush = CreateSolidBrush(RGB(74, 117, 44));

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_SAPPER, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SAPPER));

    MSG msg;

    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}


ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SAPPER));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_SAPPER);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance;

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 436, 559, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

void restart_game() {
    restart_field(&field);
    stop_timer();
    set_timer(0);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE: 
        {
            SetTimer(hWnd, 1, 500, NULL);
            SetMenu(hWnd, NULL);
            restart_field(&field);
            break;
        }
    case WM_TIMER:
        {
            InvalidateRect(hWnd, NULL, TRUE);
        }
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_ERASEBKGND: 
        {
            return TRUE;
        }
    case WM_LBUTTONDOWN:
        {
            if (!on_records_screen()) {
                on_field_click(hWnd, &field, 1);
            }
            InvalidateRect(hWnd, NULL, TRUE);
        } break;
    case WM_LBUTTONUP:
        {
            if (!on_records_screen()) {
                on_save_load_click(hWnd, &field);
                on_leaders_table_click(hWnd, &field);
            }
            else {
                on_leaders_table_return_click(hWnd, &field);
            }
        } break;
    case WM_RBUTTONDOWN:
        {
            if (!on_records_screen()) {
                on_field_click(hWnd, &field, 2);
            }
            InvalidateRect(hWnd, NULL, TRUE);
        } break;
    case WM_MOUSEMOVE:
        {
            InvalidateRect(hWnd, NULL, TRUE);
        } break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            RECT bkRect = { 0, 0, 436, 100 }; FillRect(hdc, &bkRect, headerBrush);
            if (!on_records_screen()) {
                draw_save_load_buttons(hWnd, hdc);
                draw_leaders_table_button(hWnd, hdc);
                draw_timer(hdc);
                draw_field(hdc, hWnd, &field);
            }
            else {
                RECT bkRect = { 0, 100, 436, 559 }; FillRect(hdc, &bkRect, headerBrush);
                draw_leaders_table_return_button(hWnd, hdc);
                draw_leaders_table(hdc);
            }
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
