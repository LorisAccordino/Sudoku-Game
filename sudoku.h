#ifndef SUDOKU_H_INCLUDED
#define SUDOKU_H_INCLUDED

#include "ui.h"
#include "math2D.h"

// Definizioni varie

#define BOARD_XY (Point){0, 0}
#define USER_INPUT_XY (Point){0, 21}
#define ASCII_ART_XY (Point){32, 2}
#define HINTS_XY (Point){32, 11}
#define MENU_XY (Point){0, 15}


int sudoku[9][9]; // Tabellone del sudoku


// Enumerazione dei livelli di difficoltà
enum Difficulty {
    EASY,
    MEDIUM,
    HARD
};

// Dichiarazioni di funzioni

void print_board(Point print_xy, int sudoku[9][9]);
void reset_board(int sudoku[9][9]);
void generate_random_sudoku(int sudoku[9][9], enum Difficulty difficult);
int is_valid(int sudoku[9][9], int row, int col, int num);
int validate_sudoku(int sudoku[9][9]);
int solve_board(int sudoku[9][9]);
void edit_mode(int sudoku[9][9]);
void select_cell(Point print_xy, Point cell, int highlight);

#endif // SUDOKU_H_INCLUDED
