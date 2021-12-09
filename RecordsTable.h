#pragma once
#include "framework.h"
#include "Field.h"

typedef struct {
    unsigned int win_time;
    TCHAR name[21];
    time_t date;
} Record;


typedef struct {
    Record records[11];
    int last;
} RecordsTable;

HWND get_leaders_name_field();

void set_leaders_name_field(HWND hWnd);

int on_records_screen();

void change_records_screen();

int compare_records(Record first, Record second);

int add_record(int win_time, TCHAR name[]);

void encode_record(TCHAR str[]);

void decode_record(TCHAR str[]);

void save_records();

void save_records_safe();

void load_records();

void draw_leaders_table_button(HWND hWnd, HDC hdc);

void draw_leaders_table_return_button(HWND hWnd, HDC hdc);

void draw_leaders_table_insert_button(HWND hWnd, HDC hdc);

void draw_leaders_table(HDC hdc);

void on_leaders_table_click(HWND hWnd);

void on_leaders_table_return_click(HWND hWnd);

void on_leaders_table_insert_click(HWND hWnd);
