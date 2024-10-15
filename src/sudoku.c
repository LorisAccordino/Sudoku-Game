#include "sudoku.h"
#include "common.h"

// Funzione per stampare a schermo il sudoku
void print_board(Point print_xy, int sudoku[9][9]) {
    int i, j, k;

    // Linea separatore orizzontale
    int line_length = ((9 * 2) + (3 + 1) * 2) - 1;
    char line[line_length + 1];

    for (k = 0; k < line_length; k++) {
        line[k] = '-';
    }
    line[line_length] = '\0'; // Aggiungi il terminatore di stringa

    // Stampa le etichette delle righe e delle colonne (valori da 1 a 9)
    for (i = 1; i <= 9; i++) {
        int offset = (i + 2) / 3;
        mvprintw(print_xy.y, print_xy.x + (i + offset) * 2, "%d ", i); // Etichette colonne
        mvprintw(print_xy.y + i + offset, print_xy.x, "%d ", i); // Etichette righe
    }

    // Stampa i separatori orizzontali e verticali
    for (i = 0; i <= 3; i++) {
        for (j = 0; j < 9; j++) {
            int offset = j / 3 + 2;
            mvprintw(print_xy.y + j + offset, print_xy.x + (i * 4 * 2) + 2, "|"); // Separatori verticali
        }
        mvprintw(print_xy.y + (i * 4) + 1, print_xy.x + 2, "%s", line); // Separatori orizzontali
    }

    // Stampa i numeri
    for (i = 0; i < 9; i++) {
        int y_offset = i / 3;
        for (j = 0; j < 9; j++) {
            int x_offset = j / 3 * 2;

            // Sposta il cursore nella posizione del numero da inserire
            move(print_xy.y + 2 + i + y_offset, print_xy.x + 4 + (j * 2) + x_offset);
            sudoku[i][j] != 0 ? printw("%d", sudoku[i][j]) : printw("_"); // Stampa il numero o il '_'
        }
    }

    refresh();
}

// Funzione per resettare il sudoku
void reset_board(int sudoku[9][9]) {
    reset_int_array(sudoku, 0, 9, 9);
}

// Funzione per generare numeri random nelle box diagonali 3x3 e risolvere i restanti numeri
void generate_random_sudoku(int sudoku[9][9], enum Difficulty difficulty) {
    int b, i, j;

    // Svuota sudoku
    reset_board(sudoku);

    // Array per tenere traccia dei numeri utilizzati nei box diagonali
    int used_numbers[9] = {0};

    // Itera per ogni box sulla diagonale principale (di 3 in 3)
    for (b = 0; b < 9; b += 3) {
        // Resetta i numeri usati tutti a 0
        reset_int_vector(used_numbers, 0, 9);

        for (i = b; i < 3 + b; i++) {
            for (j = b; j < 3 + b; j++) {
                int num;
                // Genera un numero casuale fino a quando non trova un numero non utilizzato
                do {
                    num = generate_random(1, 9);
                } while (used_numbers[num-1] == 1);
                used_numbers[num-1] = 1;
                sudoku[i][j] = num;
            }
        }
    }

    // Risolve il Sudoku generato
    solve_sudoku();

    // Rimuove le cifre in base alla difficoltà
    int num_to_remove = generate_random(25 + (difficulty * 10), 35 + (difficulty * 10));

    // Rimuovi le cifre casuali dal Sudoku
    i = 0;
    while (i < num_to_remove) {
        int row = generate_random(0, 8);
        int col = generate_random(0, 8);

        // Se la cifra è già stata rimossa, passa alla iterazione successiva
        if (sudoku[row][col] == 0) continue;

        sudoku[row][col] = 0; // Imposta la cifra a 0 (vuota)
        i++; // Incrementa iteratore
    }
}

// Funzione per verificare se un valore è valido per una data cella del Sudoku
int is_valid(int sudoku[9][9], int row, int col, int num) {
    int i, j;

    // Controllo riga e colonna
    for (i = 0; i < 9; i++) {
        if ((sudoku[row][i] == num && i != col) || (sudoku[i][col] == num && i != row)) return 0; // Non valido
    }

    // Controllo riquadro 3x3
    int startRow = (row / 3) * 3;
    int startCol = (col / 3) * 3;
    for (i = startRow; i < startRow + 3; i++) {
        for (j = startCol; j < startCol + 3; j++) {
            if (sudoku[i][j] == num && (i != row && j != col)) return 0; // Non valido
        }
    }

    return 1; // Valido
}

// Funzione per validare l'intero Sudoku
int validate_sudoku(int sudoku[9][9]) {
    int row, col;

    // Scorri l'intero tabellone
    for (row = 0; row < 9; row++) {
        for (col = 0; col < 9; col++) {
            // Se la cella non è vuota, controlla se il numero è valido per quella cella
            if (sudoku[row][col] != 0 && !is_valid(sudoku, row, col, sudoku[row][col])) return 0; // Il Sudoku non è valido
        }
    }

    return 1; // Il Sudoku è valido
}

// Funzione per risolvere l'intero sudoku ricorsivamente
int solve_board(int sudoku[9][9]) {
    int row, col, num;

    // Trova una cella vuota
    int trovata = 0;
    for (row = 0; row < 9; row++) {
        for (col = 0; col < 9; col++) {
            if (sudoku[row][col] == 0) {
                trovata = 1;
                break;
            }
        }
        if (trovata) break;
    }

    // Se non ci sono più celle vuote, il Sudoku è risolto
    if (!trovata) return 1;

    // Prova a inserire numeri da 1 a 9
    for (num = 1; num <= 9; num++) {
        if (is_valid(sudoku, row, col, num)) {
            sudoku[row][col] = num;

            // Ricorsione per risolvere il Sudoku
            if (solve_board(sudoku)) return 1; // Sudoku risolto

            // Se non è possibile risolvere il Sudoku con questo numero, torna indietro
            sudoku[row][col] = 0;
        }
    }

    return 0;
}

// Funzione per modificare le celle del sudoku
void edit_mode(int sudoku[9][9]) {
    Point board_xy = {0, 0};

    Point point = {0, 0};

    // Seleziona la cella inizialmente
    select_cell(board_xy, point, 1); // Seleziona la nuova cella

    Point previous_point = point;

    char key;
    while(key != ESC) {
        // Ottieni l'input dell'utente
        key = getch();

        // Aggiorna l'indice in base al tasto premuto
        switch (key) {
            case ARROW_UP:
                point.y--;
                break;
            case ARROW_DOWN:
                point.y++;
                break;
            case ARROW_LEFT:
                point.x--;
                break;
            case ARROW_RIGHT:
                point.x++;
                break;
            default:
                // Verifica se il tasto premuto è un numero
                if (key >= '0' && key <= '9') {
                    // Imposta il valore della cella corrente
                    sudoku[point.y][point.x] = key - '0';
                    print_board(board_xy, sudoku);
                    select_cell(board_xy, point, 1); // Seleziona la cella
                }

                // Passa alla iterazione successiva
                continue;
                break;
        }

        // Confina le coordinate del tabellone del sudoku
        point.x = CLAMP(point.x, 0, 9 - 1);
        point.y = CLAMP(point.y, 0, 9 - 1);

        // Se la cella selezionata è cambiata
        if (previous_point.x != point.x || previous_point.y != point.y) {
            select_cell(board_xy, previous_point, 0); // Deseleziona la cella precedente
            select_cell(board_xy, point, 1); // Seleziona la nuova cella

            // Aggiorna la cella
            previous_point = point;
        }
    }

    // Deseleziona la cella
    select_cell(board_xy, point, 0);
}

// Funzione per selezionare una cella
void select_cell(Point print_xy, Point cell, int highlight) {
    int y_offset = cell.y / 3;
    int x_offset = cell.x / 3 * 2;

    // Inverte i colori di testo e sfondo
    if (highlight) attron(COLOR_PAIR(1) | A_REVERSE);

    // Sposta il cursore nella posizione del numero da inserire
    move(print_xy.y + 2 + cell.y + y_offset, print_xy.x + 4 + (cell.x * 2) + x_offset);
    sudoku[cell.y][cell.x] != 0 ? printw("%d", sudoku[cell.y][cell.x]) : printw("_"); // Stampa il numero o il '_'

    // Ripristina i colori originali
    if (highlight) attroff(COLOR_PAIR(1) | A_REVERSE);
}
