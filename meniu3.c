#include<ncurses.h>

#include <stdlib.h>

#include <string.h>

#include"declaration.h"

// coloreaza matricea din centru pentru atunci cand 
// playerul trebuie sa isi aleaga configuratia
void coloreaza_centru(int culoare, int x, int y, WINDOW * local_win) {
    int i, j;
    i = x;
    j = y;
    wattron(local_win, COLOR_PAIR(culoare));
    mvwprintw(local_win, i * 2 + 15, j * 4 + 30, "%d%d%d%d", i, j, i, j);
    mvwprintw(local_win, i * 2 + 15 + 1, j * 4 + 30, "%d%d%d%d", i, j, i, j);
    attroff(COLOR_PAIR(culoare));
}
// afiseaza matricea din centru pentru cand isi alege
// jucatorul configuratia
void afis_tabla(int matr[10][10], WINDOW * local_win) {
    int j, k;
    for (j = 0; j < 10; j++) {
        for (k = 0; k < 10; k++) {
            if (k % 2 < 1) {
                if (j % 2 < 1) {
                    coloreaza_centru(1, j, k, local_win);

                } else {
                    coloreaza_centru(2, j, k, local_win);
                }
            } else {
                if (j % 2 > 0) {
                    coloreaza_centru(1, j, k, local_win);
                } else {
                    coloreaza_centru(2, j, k, local_win);
                }
            }

            if (matr[j][k] == 1) {
                coloreaza_centru(6, j, k, local_win);
            }
        }
    }
}
// se afiseaza optinile de nave pe rand
// la apasarea sagetilor de navigatie iar
// playerul apase pe enter ca sa isi selecteze matricea
// cu care va juca jocul
WINDOW * alegere(int x, int y, int width, int height,
            int matr[10][10], int argc, char ** argv) {
    WINDOW * local_win;
    local_win = newwin(height, width, y, x);
    init_pair(1, COLOR_YELLOW, COLOR_YELLOW);
    init_pair(2, COLOR_WHITE, COLOR_WHITE);
    init_pair(3, COLOR_YELLOW, COLOR_WHITE);
    init_pair(4, COLOR_CYAN, COLOR_WHITE);
    init_pair(5, COLOR_RED, COLOR_RED);
    init_pair(6, COLOR_GREEN, COLOR_GREEN);
    init_pair(7, COLOR_MAGENTA, COLOR_MAGENTA);
    int i, ch;
    noecho(); 
    keypad(local_win, TRUE); 
    curs_set(0); 

    mvwprintw(local_win, 3, 45, "Alege-ti configuratia");
    i = 1;
    citire_fisier(matr, argv[i]);
    afis_tabla(matr, local_win);
    while ((ch = wgetch(local_win))) {
        switch (ch) {
        case KEY_RIGHT:
            i = i + 1;
            i = (i == argc) ? 1 : i;
            wattron(local_win, COLOR_PAIR(3));
            citire_fisier(matr, argv[i]);
            afis_tabla(matr, local_win);
            break;
        case KEY_LEFT:
            i = i - 1;
            i = (i == 0) ? argc - 1 : i;
            wattron(local_win, COLOR_PAIR(3));
            citire_fisier(matr, argv[i]);
            afis_tabla(matr, local_win);
            break;
        case 10:
            wrefresh(local_win);
            wclear(local_win);
            endwin();
            return 0;
            break;
        }
    }

    return local_win;
}
// afiseaza meniul alocat cu sirulrile de caractere alocate dinamic
// navigheaza prin meniu iar daca butonul de iesire este apasat
// se iese din meniu iar daca se poate da resume game optiunea este 
// activa si se poate apasa pe ea iar daca nu atunci se trece peste ea
// iar daca se apasa new game se trece mai intai in functia de selectat
// matricea jucatorului si dupa in joc efectiv 

WINDOW * menu(int x, int y, int width, int height, int * resume,
            int matr[10][10], int eu[10][10], int argc, char ** argv) {
    WINDOW * local_win;
    local_win = newwin(height, width, y, x);

    init_pair(1, COLOR_YELLOW, COLOR_YELLOW);
    init_pair(2, COLOR_CYAN, COLOR_CYAN);
    init_pair(3, COLOR_YELLOW, COLOR_WHITE);
    init_pair(4, COLOR_CYAN, COLOR_WHITE);

    char * item = (char * ) calloc(13, sizeof(char));
    char ** list = (char ** ) calloc(3, sizeof(char * ));
    list[0] = (char * ) calloc(9, sizeof(char));
    list[1] = (char * ) calloc(12, sizeof(char));
    list[2] = (char * ) calloc(5, sizeof(char));
    // char list[3][13] = { "New Game", "Resume Game", "Quit"};
    strcpy(list[0], "New Game");
    strcpy(list[1], "Resume Game");
    strcpy(list[2], "Quit");
    int ch, i = 0, nave_sparte[2], nimerite, nimeritecalc;

    initscr();
    local_win = newwin(height, width, y, x);
    box(local_win, 0, 0);
    wrefresh(local_win);
    refresh();
    wclear(local_win);
    wrefresh(local_win);
    nave_sparte[0] = 0;
    nave_sparte[1] = 0;
    nimerite = 0;
    nimeritecalc = 0;
    for (i = 0; i < 3; i++) {
        if (i == 0)
            wattron(local_win, A_STANDOUT);
        else
            wattroff(local_win, A_STANDOUT);
        sprintf(item, "%s", list[i]);
        mvwprintw(local_win, i + 27, 40, "%s", item);
    }

    wrefresh(local_win);
    i = 0;
    noecho();
    keypad(local_win, TRUE);
    curs_set(0);
    while ((ch = wgetch(local_win))) {
        sprintf(item, "%s", list[i]);
        mvwprintw(local_win, i + 27, 40, "%s", item);

        switch (ch) {
        case KEY_UP:
            i--;
            i = (i < 0) ? 2 : i;
            i = (i == 1 && !*resume) ? 0 : i;
            break;
        case KEY_DOWN:
            i++;
            i = (i > 2) ? 0 : i;
            i = (i == 1 && !*resume) ? 2 : i;
            break;
        case 10:
            // create a new window
            sprintf(item, "%s", list[i]);
            wrefresh(local_win);
            wclear(local_win);
            wrefresh(local_win);
            start_color();

            if (i == 0) {
                * resume = 0;
                nave_sparte[0] = 0;
                nave_sparte[1] = 0;
                nimerite = 0;
                nimeritecalc = 0;
                alegere(0, 0, 120, 50, eu, argc, argv);
                wrefresh(local_win);
                wclear(local_win);
                refresh();
                clear();
                wrefresh(local_win);
                wclear(local_win);
                create_board(0, 0, 120, 50, resume, matr,
                     eu, nave_sparte, & nimerite, & nimeritecalc);

                wrefresh(local_win);
                wclear(local_win);
                refresh();
                clear();
                wrefresh(local_win);
                wclear(local_win);
                for (i = 0; i < 3; i++) {
                    if (i == 0)
                        wattron(local_win, A_STANDOUT);
                    else
                        wattroff(local_win, A_STANDOUT);
                    sprintf(item, "%s", list[i]);
                    mvwprintw(local_win, i + 27, 40, "%s", item);
                }
                wrefresh(local_win);
                i = 0;

            } else if (i == 1) {
                create_board(0, 0, 120, 50, resume, matr, eu, nave_sparte,
                             & nimerite, & nimeritecalc);
                wrefresh(local_win);
                wclear(local_win);
                refresh();
                clear();
                wrefresh(local_win);
                wclear(local_win);
                for (i = 0; i < 3; i++) {
                    if (i == 1)
                        wattron(local_win, A_STANDOUT);
                    else
                        wattroff(local_win, A_STANDOUT);
                    sprintf(item, "%s", list[i]);
                    mvwprintw(local_win, i + 27, 40, "%s", item);
                }
                i = 1;
                wrefresh(local_win);

            } else if (i == 2) {
                free(item);
                for (i = 0; i < 3; i++) {
                    free(list[i]);
                }
                free(list);
                endwin();
                return 0;
            }
        }
        wattron(local_win, A_STANDOUT);

        sprintf(item, "%s", list[i]);
        mvwprintw(local_win, i + 27, 40, "%s", item);
        wattroff(local_win, A_STANDOUT);
    }

    return local_win;
}
int main(int argc, char * argv[]) {
    initscr();
    if (has_colors() == false) {
        endwin();
        printf("No color support!\n");
        return -1;
    }
    start_color();
    int resume, matr[10][10], eu[10][10], i;
    // int **matr = (int **)calloc(10 , sizeof(int*));
    // int **eu = (int **)calloc(10 , sizeof(int*));
    // for(i = 0; i < 10; i++){
    //     eu[i] = (int *)calloc(10 , sizeof(int));
    //     matr[i] = (int *)calloc(10 , sizeof(int));
    // }
    resume = 0;
    // daca nu s-au dat argumente se returneaza eroare
    if (argc == 1) {
        printf("[Eroare]: Nu s-au dat argumente de comanda.");
        return 1;
    }
    //daca nu s-a dat vreo configuratie buna se returneaza eroare
    for (i = 1; i < argc; i++) {
        if (verificare_fisier(argv[i]) == 1) {
            return 1;
        }
    }
    flames(); // se afiseaza ecranul de inceput
    //se curata dupa el
    refresh();
    clear();
    refresh();
    // init_pair(1, COLOR_RED, COLOR_BLACK);
    // attron(COLOR_PAIR(1));
    // printw("This is a test!");
    // attroff(COLOR_PAIR(1));
    //se afiseaza meniul
    menu(0, 0, 120, 50, & resume, matr, eu, argc, argv);
    // for(i = 0; i < 10; i++){
    //     free(eu[i]);
    //     free(matr[i]);
    // }

    // free(matr);
    // free(eu);
    endwin();
    return 1;
}
