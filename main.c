#include "common.h"
#include "ui.h"
#include "sudoku.h"


void reset_sudoku() {
    if (are_you_sure(USER_INPUT_XY, "Stai per svuotare il sudoku. Sei sicuro?", "Sì", "No")) {
        reset_board(sudoku);
        print_board(BOARD_XY, sudoku);
    }
}

void generate_sudoku() {
    // Livelli di difficoltà
    char *choices[] = {"Facile", "Medio", "Difficile"};
    enum Difficulty difficulty = choice_in_line(USER_INPUT_XY, "Scegli un livello di difficoltà: ", choices, ARRAY_SIZE(choices));

    if (difficulty == -1) return; // Tasto ESC premuto

    // Genera un nuovo sudoku random
    generate_random_sudoku(sudoku, difficulty);
    print_board(BOARD_XY, sudoku);
}

void edit_sudoku() {
    edit_mode(sudoku);
}

void solve_sudoku() {
    // Se supera la validazione e se supera la risoluzione
    if (validate_sudoku(sudoku) && solve_board(sudoku)) { // &&: operatore valutato a corto circuito
        print_board(BOARD_XY, sudoku);
        return;
    }

    // Sudoku non risolubile
    info(USER_INPUT_XY, "Sudoku impossibile da risolvere!", "Ok");
}

void exit_game() {
    if (are_you_sure(USER_INPUT_XY, "Stai per uscire dal gioco. Sei sicuro?", "Sì", "No")) {
        exit(0);
    }
}





int main() {
    // Init vari
    init_ncurses(); // Inizializza ncurses
    set_window(70, 25, "Sudoku"); // Imposta dimensione e titolo della finestra
    init_random(); // Inizializza il seed del generatore random


    // Stampa la griglia del Sudoku
    reset_board(sudoku);
    print_board(BOARD_XY, sudoku);


    // Stampa l'ASCII art a schermo
    char *ascii_art = read_ascii_art_from_file("ascii_art.txt"); // Scritta in ASCII art
    int num_lines;
    char** text = split_string(ascii_art, &num_lines); // Dividi la scritta in linee
    print_multi_line_text(ASCII_ART_XY, text, num_lines);


    // Stampa i suggerimenti
    char *hints[] = {
        " - Naviga i menu con i tasti freccia",
        " - Premi [ESC] per uscire dai menu"
    };
    print_multi_line_text(HINTS_XY, hints, ARRAY_SIZE(hints));


    // Stampa il menu delle opzioni
    MenuItem main_menu_items[] = {
        {"Svuota il sudoku", reset_sudoku},
        {"Genera il sudoku", generate_sudoku},
        {"Modifica il sudoku", edit_sudoku},
        {"Risolvi il sudoku", solve_sudoku},
        {"Esci", exit_game}
    };
    choice_menu(MENU_XY, main_menu_items, ARRAY_SIZE(main_menu_items), 0, 0);

    // Pulisci e termina ncurses
    cleanup_ncurses();

    return 0;
}
