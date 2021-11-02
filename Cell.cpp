#include "Cell.h"
#include "Field.h"
#include "framework.h"
#include <stdlib.h>


const TCHAR* numbers[9] = { TEXT(" "), TEXT("1"), TEXT("2"), TEXT("3"), 
							TEXT("4"), TEXT("5"), TEXT("6"), TEXT("7"), TEXT("8") };
const TCHAR* bomb = TEXT("💣");
const TCHAR* flag = TEXT("🚩");
const COLORREF colors[9] = { RGB(0, 0, 0), RGB(0, 0, 255), RGB(0, 255, 0), RGB(255, 0, 0),
							 RGB(200, 0, 200), RGB(240, 240, 0), RGB(255, 165, 0), RGB(150, 0, 150), RGB(0, 0, 0) };
const COLORREF odd_backgound = RGB(229, 194, 159);
const COLORREF even_background = RGB(215, 184, 153);

void draw_odd_backgournd(HDC hdc) {

}