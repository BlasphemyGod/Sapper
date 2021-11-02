#include "Cell.h"
#include "Field.h"
#include "framework.h"
#include <stdlib.h>
#define CELL_WIDTH 50
#define CELL_HEIGHT 50


const TCHAR* numbers[9] = { TEXT(" "), TEXT("1"), TEXT("2"), TEXT("3"), 
							TEXT("4"), TEXT("5"), TEXT("6"), TEXT("7"), TEXT("8") };
const TCHAR* bomb = TEXT("💣");
const TCHAR* flag = TEXT("🚩");
const COLORREF colors[9] = { RGB(0, 0, 0), RGB(0, 0, 255), RGB(0, 255, 0), RGB(255, 0, 0),
							 RGB(200, 0, 200), RGB(240, 240, 0), RGB(255, 165, 0), RGB(150, 0, 150), RGB(0, 0, 0) };
const COLORREF even_background = RGB(229, 194, 159);
const COLORREF odd_background = RGB(215, 184, 153);
const COLORREF even_curtain = RGB(170, 215, 81);
const COLORREF odd_curtain = RGB(162, 209, 73);
const HFONT hFont = CreateFont(-40, 0, 0, 0, 0, ANSI_CHARSET, 0, 0, 0, 0, 0, 0, 0, TEXT("Arial"));

void draw_even_backgournd(HDC hdc, int x, int y) {
	HPEN hPen = CreatePen(PS_SOLID, 0, even_background);
	HBRUSH hBrush = CreateSolidBrush(even_background);
	SelectObject(hdc, hPen);
	SelectObject(hdc, hBrush);
	Rectangle(hdc, x, y, x + CELL_WIDTH, y + CELL_HEIGHT);
	DeleteObject(hPen);
	DeleteObject(hBrush);
}

void draw_odd_background(HDC hdc, int x, int y) {
	HPEN hPen = CreatePen(PS_SOLID, 0, odd_background);
	HBRUSH hBrush = CreateSolidBrush(odd_background);
	SelectObject(hdc, hPen);
	SelectObject(hdc, hBrush);
	Rectangle(hdc, x, y, x + CELL_WIDTH, y + CELL_HEIGHT);
	DeleteObject(hPen);
	DeleteObject(hBrush);
}

void draw_number(HDC hdc, int x, int y, int number) {
	SetTextColor(hdc, colors[number]);
	SetBkMode(hdc, TRANSPARENT);
	SelectObject(hdc, hFont);
	RECT rect = { x, y, x + CELL_WIDTH, y + CELL_HEIGHT };
	DrawText(hdc, numbers[number], lstrlen(numbers[number]), &rect, DT_CENTER);
}

void draw_bomb(HDC hdc, int x, int y) {
	SetTextColor(hdc, RGB(0, 0, 0));
	SetBkMode(hdc, TRANSPARENT);
	SelectObject(hdc, hFont);
	RECT rect = { x, y, x + CELL_WIDTH, y + CELL_HEIGHT };
	DrawText(hdc, bomb, lstrlen(bomb), &rect, DT_CENTER);
}

void draw_even_curtain(HDC hdc, int x, int y) {
	HPEN hPen = CreatePen(PS_SOLID, 0, even_curtain);
	HBRUSH hBrush = CreateSolidBrush(even_curtain);
	SelectObject(hdc, hPen);
	SelectObject(hdc, hBrush);
	Rectangle(hdc, x, y, x + CELL_WIDTH, y + CELL_HEIGHT);
	DeleteObject(hPen);
	DeleteObject(hBrush);
}

void draw_odd_curtain(HDC hdc, int x, int y) {
	HPEN hPen = CreatePen(PS_SOLID, 0, odd_curtain);
	HBRUSH hBrush = CreateSolidBrush(odd_curtain);
	SelectObject(hdc, hPen);
	SelectObject(hdc, hBrush);
	Rectangle(hdc, x, y, x + CELL_WIDTH, y + CELL_HEIGHT);
	DeleteObject(hPen);
	DeleteObject(hBrush);
}

void draw_flag(HDC hdc, int x, int y) {
	SetTextColor(hdc, RGB(255, 0, 0));
	SetBkMode(hdc, TRANSPARENT);
	SelectObject(hdc, hFont);
	RECT rect = { x, y, x + CELL_WIDTH, y + CELL_HEIGHT };
	DrawText(hdc, flag, lstrlen(flag), &rect, DT_CENTER);
}