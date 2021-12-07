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

Field field = new_field(0, 100, 15);
HBRUSH headerBrush = CreateSolidBrush(RGB(74, 117, 44));
int game_ended = 0;

void switch_on_leaders_table() {
    if (!field.is_first_cell && !game_ended)
        stop_timer();
    if (game_ended)
        ShowWindow(get_leaders_name_field(), SW_SHOWNORMAL);
    change_records_screen();
}

void switch_off_leaders_table() {
    if (!field.is_first_cell && !game_ended)
        unpause_timer();
    ShowWindow(get_leaders_name_field(), SW_HIDE);
    change_records_screen();
}

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
    game_ended = 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE: 
        {
            set_leaders_name_field(CreateWindowW(
                TEXT("edit"), TEXT("Имя игрока"),
                WS_CHILD | WS_VISIBLE | WS_BORDER | ES_RIGHT, 120, 90, 195, 20,
                hWnd, 0, hInst, NULL
            ));
            ShowWindow(get_leaders_name_field(), SW_HIDE);
            SetTimer(hWnd, 1, 500, NULL);
            SetMenu(hWnd, NULL);
            restart_field(&field);
            load_records();
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
                int result = on_field_click(hWnd, &field, 1);
                if (result == -1) {
                    MessageBox(hWnd, TEXT("YOU LOSE!"), TEXT("Oh my God!"), MB_OK);
                    restart_game();
                }
                else if (result == 1) {
                    game_ended = 1;
                    switch_on_leaders_table();
                }
            }
            InvalidateRect(hWnd, NULL, TRUE);
        } break;
    case WM_LBUTTONUP:
        {
            if (!on_records_screen()) {
                on_save_load_click(hWnd, &field);
                on_leaders_table_click(hWnd);
            }
            else {
                if (game_ended) {
                    on_leaders_table_insert_click(hWnd);
                }
                on_leaders_table_return_click(hWnd);
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
                if (game_ended) {
                    draw_leaders_table_insert_button(hWnd, hdc);
                }
            }
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        save_records_safe();
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
