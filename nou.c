#include <ncurses.h>

#include <stdio.h>

#include <stdlib.h>

#include <time.h>

#include"declaration.h"

#define LINII 53
#define COLOANE 204
#define BUFMAX 1005
// initializez matriccea cu 0
void init_matr(int matr[10][10]) {
    int i, j;
    for (i = 0; i < 10; i++) {
        for (j = 0; j < 10; j++) {
            matr[i][j] = 0;
        }
    }
}
// fac o copie a matricei care imi va trebui pentru pastrarea
// contorului cand se va efectua functia de randomizare
void copie_matr(int matr[10][10], int copie[10][10]) {
    int i, j;
    for (i = 0; i < 10; i++) {
        for (j = 0; j < 10; j++) {
            copie[i][j] = matr[i][j];
        }
    }
}

// o functie in care printez matricea
void print_matr(int matr[10][10]) {
    int i, j;
    for (i = 0; i < 10; i++) {
        for (j = 0; j < 10; j++) {
            printf("%d", matr[i][j]);
        }
        printf("\n");
    }
}
// functia in care citesc din fisier aranjamentele
int citire_fisier(int eu[10][10], char a[30]) {
    FILE * pFile;

    pFile = fopen(a, "r");
    if (pFile == NULL) {
        fprintf(stderr, "[Eroare]: Fisierul %s nu poate fi deschis", a);
        return 1;
    } else {
        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 10; j++) {
                fscanf(pFile, "%d", & eu[i][j]);
            }
        }
        fclose(pFile);
        return 1;
    }
}
// functie pe care o folosesc inainte sa inceapa jocul in care verific daca
// fisierul dat ca argument poate fi deschis
int verificare_fisier(char a[30]) {
    FILE * pFile;

    pFile = fopen(a, "r");
    if (pFile == NULL) {
        fprintf(stderr, "[Eroare]: Fisierul %s nu poate fi deschis", a);
        return 1;
    } else {
        fclose(pFile);
        return 0;
    }
}
// functia aceasta face un patrat de o culoare precizata pe matricea calculator
void coloreaza(int culoare, int x, int y, WINDOW * local_win) {
    int i, j;
    i = x;
    j = y;
    wattron(local_win, COLOR_PAIR(culoare));
    mvwprintw(local_win, i * 2 + 5, j * 4 + 5, "%d%d%d%d", i, j, i, j);
    mvwprintw(local_win, i * 2 + 5 + 1, j * 4 + 5, "%d%d%d%d", i, j, i, j);
    attroff(COLOR_PAIR(culoare));
}
// functia aceasta face un patrat de o culoare precizata pe matricea mea
void coloreaza_eu(int culoare, int x, int y, WINDOW * local_win) {
    int i, j;
    i = x;
    j = y;
    wattron(local_win, COLOR_PAIR(culoare));
    mvwprintw(local_win, i * 2 + 5, j * 4 + 50, "%d%d%d%d", i, j, i, j);
    mvwprintw(local_win, i * 2 + 5 + 1, j * 4 + 50, "%d%d%d%d", i, j, i, j);
    attroff(COLOR_PAIR(culoare));
}
// functia aceasta imi pune o corabie de o lungime data si de o directie data
// pe matricea calculatorului si imi blocheaza casutele cu nava si casutele 
// invecinate pentru a nu mai putea fi folosite
// 1 - pe bucata e o corabie
// 2 - corabia e intr o vecinatate
void scriere_matrice(int matr[10][10], int dir, int x, int y, int nrpatr) {
    int i = 0;
    if (x - 1 >= 0) matr[x - 1][y] = 2; // in stanga
    if (y - 1 >= 0) matr[x][y - 1] = 2; // in sus
    if (x + 1 < 10) matr[x + 1][y] = 2; // in dreapta
    if (y + 1 < 10) matr[x][y + 1] = 2; // in stanga

    if (x + 1 < 10 && y + 1 < 10) matr[x + 1][y + 1] = 2; // in dreapta jos
    if (x + 1 < 10 && y - 1 >= 0) matr[x + 1][y - 1] = 2; // in stanga jos
    if (x - 1 >= 0 && y + 1 < 10) matr[x - 1][y + 1] = 2; // in dreapta sus
    if (x - 1 >= 0 && y - 1 >= 0) matr[x - 1][y - 1] = 2; // in stanga sus

    for (i = 0; i < nrpatr; i++) {
        if (dir == 1) {
            if (y - 1 >= 0) matr[x + i][y - 1] = 2;
            matr[x + i][y] = 1;
            if (y + 1 < 10) matr[x + i][y + 1] = 2;
            if (x + i + 1 < 10) matr[x + i + 1][y] = 2;
            if (x + i + 1 < 10 && y + 1 < 10) matr[x + i + 1][y + 1] = 2;
            if (x + i + 1 < 10 && y - 1 >= 0) matr[x + i + 1][y - 1] = 2;
        } else {
            if (x - 1 >= 0) matr[x - 1][y + i] = 2;
            matr[x][y + i] = 1;
            if (x + 1 < 10) matr[x + 1][y + i] = 2;
            if (y + i + 1 < 10) matr[x][y + i + 1] = 2;
            if (y + i + 1 < 10 && x + 1 < 10) matr[x + 1][y + i + 1] = 2;
            if (y + i + 1 < 10 && x - 1 >= 0) matr[x - 1][y + i + 1] = 2;
        }
    }
}
// aceasta functie parcurge de la un punct din matrice pe directia precizata
// pentru a vedea daca toate patratele sunt libere pentru a se pune o nava pe acestea
int parcurgere_matrice(int matr[10][10], int dir, int x, int y, int nrpatr) {
    int i = 0;
    for (i = 0; i < nrpatr; i++) {
        if (dir == 0) {
            if (y + nrpatr < 10) {
                if (matr[x][y + i] != 0) {
                    return 0;
                }
            } else {
                return 0;
              }
        } else {
            if (x + nrpatr < 10) {
                if (matr[x + i][y] != 0) {
                    return 0;
                }
            } else {
                return 0;
            }
        }
    }
    return 1;
}
// aceasta functie in parcurge navele la finalul jocului
// si vede daca sunt sparte complet sau partial
int parcurgere_nava(int matr[10][10], int x, int y, int dir) {
    int sparta;
    sparta = 1;
    matr[x][y] = 0;
    if (dir == 0) { // parcurgere pe orizontala
        if (matr[x][y + 1] == 3) {
            return sparta * parcurgere_nava(matr, x, y + 1, 0);
        } else if (matr[x][y + 1] == 1) {
            sparta = 2;
            return sparta * parcurgere_nava(matr, x, y + 1, 0);
        } else if (matr[x][y - 1] == 1) {
            sparta = 2;
            return sparta * parcurgere_nava(matr, x, y - 1, 0);
        }

        return -1;
    } else { // parcurgere pe verticala
        if (matr[x + 1][y] == 3) {
            return sparta * parcurgere_nava(matr, x + 1, y, 1);
        } else if (matr[x + 1][y] == 1) {
            sparta = 2;
            return sparta * parcurgere_nava(matr, x + 1, y, 1);
        } else if (matr[x - 1][y] == 1) {
            sparta = 2;
            return sparta * parcurgere_nava(matr, x - 1, y, 1);
        }
        return -1;
    }
}
// aceasta functie imi parcurge pe rand
// navele din matricea copie a matricei initiale
// in momentul in care s-a apasat pe butonul de randomize
// pentru a vedea cate parti mai are o nava
int parcurgere_rand(int matr[10][10], int x, int y, int dir) {
    matr[x][y] = 0;
    if (dir == 0) { // parcurgere pe orizontala
        if (matr[x][y + 1] == 3 && y + 1 < 10) {
            return 0 + parcurgere_rand(matr, x, y + 1, 0);
        } else if (matr[x][y + 1] == 1 && y + 1 < 10) {
            return 1 + parcurgere_rand(matr, x, y + 1, 0);
        } else if (matr[x][y - 1] == 3 && y - 1 >= 0) {
            return 0 + parcurgere_rand(matr, x, y - 1, 0);
        }

        return 0;
    } else { // parcurgere pe verticala
        if (matr[x + 1][y] == 3 && x + 1 < 10) {
            return 0 + parcurgere_rand(matr, x + 1, y, 1);
        } else if (matr[x + 1][y] == 1 && x + 1 < 10) {
            return 1 + parcurgere_rand(matr, x + 1, y, 1);
        } else if (matr[x - 1][y] == 3 && x - 1 >= 0) {
            return 0 + parcurgere_rand(matr, x - 1, y, 1);
        }
        return 0;
    }
}
// acesta functie imi alege random un punct de pornire neocupat
// si o directie si parcurge matricea pe directia respectiva
// iar daca sunt libere locurile scrie nava pentru toate navele 
// ramase dupa randomizare
void plasare_random(int matr[10][10], int nave[13], int nrnave) {
    int i, directie, rx, ry;
    srand(time(NULL));

    init_matr(matr);
    for (i = 0; i < nrnave; ++i) {
        directie = rand() % 2; // 1-vertical 0-orizontal
        rx = rand() % 10;
        ry = rand() % 10;
        while (nave[i]) {
            directie = (rand() % 90) % 2; // 1-vertical 0-orizontal
            rx = rand() % 10;
            ry = rand() % 10;
            if (parcurgere_matrice(matr, directie, rx, ry, nave[i]) == 1) {
                scriere_matrice(matr, directie, rx, ry, nave[i]);
                nave[i] = 0;
            }
        }
    }
}
// aceasta functie face randomizarea
// initial verifica daca gaseste o bucata de nava intacta
// dupa parcurge nava pentru a afla numarul de bucati nerupte
// si le pune intr-un vector in care fiecare pozitie reprezinta o nava
// iar dupa plaseaza random fiecare nava
void randomizare(int matr[10][10], WINDOW * local_win) {
    int i, j, nrnave;
    nrnave = 0;
    int * nave = (int * ) calloc(11, sizeof(int));
    for (i = 0; i < 10; i++) {
        for (j = 0; j < 10; j++) {
            if (matr[i][j] == 1) {
                if ((matr[i][j + 1] == 1 && j + 1 < 10) ||
                (matr[i][j + 1] == 3 && j + 1 < 10) ||
                (matr[i][j - 1] == 3 && j - 1 >= 0)) { // orizontal
                    nave[nrnave] = parcurgere_rand(matr, i, j, 0) + 1;
                    nrnave++;
                } else if ((matr[i + 1][j] == 1 && i + 1 < 10) ||
                (matr[i + 1][j] == 3 && i + 1 < 10) || (matr[i - 1][j] == 3
                && i - 1 >= 0)) { // vertical
                    nave[nrnave] = parcurgere_rand(matr, i, j, 1) + 1;
                    nrnave++;

                } else {
                    nave[nrnave] = 1;
                    nrnave++;
                    matr[i][j] = 0;
                }
            } else if (matr[i][j] == 2 || matr[i][j] == 4) {
                matr[i][j] = 0;
            }
        }
    }
    plasare_random(matr, nave, nrnave);
    free(nave);
}
// aceasta functie imi face contorul navelor sparte inainte sa se apese
// butonul de randomizare daca final=0 iar daca final=1
// atunci imi face navele sparte si navele sparte partial
// si le afiseaza
int verificare_pierzator(int matr[10][10], WINDOW * local_win,
    int nave_sparte, int final, int loc) {
    int i, j, nave_sparte_partial;
    nave_sparte_partial = 0;
    for (i = 0; i < 10; i++) {
        for (j = 0; j < 10; j++) {
            if (matr[i][j] == 3) {
                if (matr[i][j + 1] == 1 || matr[i][j + 1] == 3 ||
                matr[i][j - 1] == 1) { // orizontal
                    if (parcurgere_nava(matr, i, j, 0) == -1)
                        nave_sparte++;
                    else
                        nave_sparte_partial++;

                } else if (matr[i + 1][j] == 1 || matr[i + 1][j] == 3
                || matr[i - 1][j] == 1) { // vertical
                    if (parcurgere_nava(matr, i, j, 1) == -1)
                        nave_sparte++;
                    else
                        nave_sparte_partial++;
                } else {
                    nave_sparte++;
                }
            }
        }
    }
    if (final) {
        mvwprintw(local_win, 30, 50 - loc,
            " Nave sparte: %d ", nave_sparte);
        mvwprintw(local_win, 31, 50 - loc,
            " Nave sparte partial: %d ", nave_sparte_partial);
        mvwprintw(local_win, 30, 5 + loc, " Nave sparte: 10 ");
        mvwprintw(local_win, 31, 5 + loc, " Nave sparte partial: 0 ");
    }

    return nave_sparte;
}
// aceasta functie imi genereaza asezarea calculatorului
// si imi pune toate navele pe matrice
void generare_matrice(int matr[10][10]) {
    int * nave = (int * ) calloc(4, sizeof(int));
    nave[0] = 4;
    nave[1] = 6;
    nave[2] = 6;
    nave[3] = 4;

    int i, directie, rx, ry;
    srand(time(NULL));

    init_matr(matr);
    for (i = 0; i < 4; ++i) {
        directie = rand() % 2; // 1-vertical 0-orizontal
        rx = rand() % 10;
        ry = rand() % 10;
        while (nave[i]) {
            directie = (rand() % 90) % 2; // 1-vertical 0-orizontal
            rx = rand() % 10;
            ry = rand() % 10;
            if (parcurgere_matrice(matr, directie, rx, ry, 4 - i) == 1) {
                scriere_matrice(matr, directie, rx, ry, 4 - i);
                nave[i] -= 4 - i;
            }
        }
    }
    free(nave);
}
// aceasta functie imi initializeaza grafica jocului
// punandu-mi regulile si colorand matricile
void init_joc(WINDOW * local_win, int eu[10][10], int matr[10][10]) {
    int i, j;
    init_pair(1, COLOR_YELLOW, COLOR_YELLOW);
    init_pair(2, COLOR_WHITE, COLOR_WHITE);
    init_pair(3, COLOR_YELLOW, COLOR_WHITE);
    init_pair(4, COLOR_CYAN, COLOR_WHITE);
    init_pair(5, COLOR_RED, COLOR_RED);
    wattroff(local_win, COLOR_PAIR(3));
                wattroff(local_win, COLOR_PAIR(1));
                wattroff(local_win, COLOR_PAIR(2));
                wattroff(local_win, COLOR_PAIR(4));
                wattroff(local_win, COLOR_PAIR(5));
                wattroff(local_win, COLOR_PAIR(6));
                wattroff(local_win, COLOR_PAIR(7));
    mvwprintw(local_win, 45, 1,
        "miscare- sagetile de la tastatura  | r-randomize  | d- destroy in advance  | q- quit(back to menu)  ");
    mvwprintw(local_win, 4, 5, "Player");
    mvwprintw(local_win, 4, 50, "Calculator");

    for (i = 0; i < 10; i++) {
        for (j = 0; j < 10; j++) {
            if (j % 2 < 1) {
                if (i % 2 < 1) {
                    coloreaza(1, i, j, local_win);
                    coloreaza_eu(1, i, j, local_win);

                } else {
                    coloreaza(2, i, j, local_win);
                    coloreaza_eu(2, i, j, local_win);
                }
            } else {
                if (i % 2 > 0) {
                    coloreaza(1, i, j, local_win);
                    coloreaza_eu(1, i, j, local_win);
                } else {
                    coloreaza(2, i, j, local_win);
                    coloreaza_eu(2, i, j, local_win);
                }
            }

            if (eu[i][j] == 1) {
                coloreaza_eu(6, i, j, local_win);
            }
            if (eu[i][j] == 3) {
                coloreaza_eu(7, i, j, local_win);
            }
            if (matr[i][j] == 3) {
                coloreaza(6, i, j, local_win);
            }
            if (eu[i][j] == 4) {
                coloreaza_eu(5, i, j, local_win);
            }
            if (matr[i][j] == 4) {
                coloreaza(5, i, j, local_win);
            }
        }
    }
}
// aceasta functie este "nucleul" jocului
// este cea apelata de meniu
// in care daca s-a apasat pe new game se genereaza o matrice
// si in care se manevreaza cu un switch ce apasa jucatorul
WINDOW * create_board(int x, int y, int width,
    int height, int * resume, int matr[10][10], int eu[10][10],
    int nave_sparte[2], int * nimerite, int * nimeritecalc) {
    WINDOW * local_win;
    int rx2, ry2, rx, ry, copie[10][10];
    int ch;
    int ok, jocgata;
    if ( !*resume ) {
        generare_matrice(matr);
    }
    local_win = newwin(height, width, y, x);
    init_pair(1, COLOR_YELLOW, COLOR_YELLOW);
    init_pair(2, COLOR_CYAN, COLOR_CYAN);
    init_pair(3, COLOR_CYAN, COLOR_CYAN);
    init_pair(4, COLOR_CYAN, COLOR_CYAN);
    init_pair(5, COLOR_RED, COLOR_RED);
    init_pair(6, COLOR_GREEN, COLOR_GREEN);
    init_pair(7, COLOR_MAGENTA, COLOR_MAGENTA);

    init_joc(local_win, eu, matr);
    jocgata = 0;
    srand(time(NULL));
    rx = rand() % 10;
    ry = rand() % 10;
    coloreaza(3, rx, ry, local_win);

    wrefresh(local_win);
    wattron(local_win, COLOR_PAIR(4));
    * resume == 0 ? mvwprintw(local_win, 0, 45, "Joc nou")
    : mvwprintw(local_win, 0, 45, "Joc reluat");

    noecho(); // disable echoing of characters on the screen
    keypad(local_win, TRUE); // enable keyboard input for the window.
    curs_set(0); // hide the default screen cursor.

    while ((ch = wgetch(local_win))) {
        switch (ch) {
        case KEY_UP:
        // se coloreaza cu alb sau negru in functie de pozitie
            ((rx % 2 < 1 && ry % 2 < 1) || (rx % 2 > 0 && ry % 2 > 0)) ?
                coloreaza(1, rx, ry, local_win):
                coloreaza(2, rx, ry, local_win);
            // daca era nimerita corect se afiseaza verde
            // daca era nimerita gresit rosu
            if (matr[rx][ry] == 3)
                coloreaza(6, rx, ry, local_win);
            else if (matr[rx][ry] == 4)
                coloreaza(5, rx, ry, local_win);
            rx--;
            rx = (rx < 0) ? 9 : rx; // daca s-a dus prea sus o ia de la ultima pozitie
            ((rx % 2 < 1 && ry % 2 < 1) || (rx % 2 > 0 && ry % 2 > 0)) ?
                coloreaza(3, rx, ry, local_win):
                coloreaza(4, rx, ry, local_win);
            break;
        case KEY_DOWN:
        // analog ca la key up
            ((rx % 2 < 1 && ry % 2 < 1) || (rx % 2 > 0 && ry % 2 > 0)) ?
                coloreaza(1, rx, ry, local_win):
                coloreaza(2, rx, ry, local_win);
            if (matr[rx][ry] == 3)
                coloreaza(6, rx, ry, local_win);
            else if (matr[rx][ry] == 4)
                coloreaza(5, rx, ry, local_win);
            rx++;
            rx = (rx > 9) ? 0 : rx;
            // se coloreaza patratica actuala
            ((rx % 2 < 1 && ry % 2 < 1) || (rx % 2 > 0 && ry % 2 > 0)) ?
                coloreaza(3, rx, ry, local_win):
                coloreaza(4, rx, ry, local_win);
            break;
        case KEY_LEFT:
        // analog ca la key up
            ((rx % 2 < 1 && ry % 2 < 1) || (rx % 2 > 0 && ry % 2 > 0)) ?
                coloreaza(1, rx, ry, local_win):
                coloreaza(2, rx, ry, local_win);
            if (matr[rx][ry] == 3)
                coloreaza(6, rx, ry, local_win);
            else if (matr[rx][ry] == 4)
                coloreaza(5, rx, ry, local_win);
            ry--;
            ry = (ry < 0) ? 9 : ry;
            ((rx % 2 < 1 && ry % 2 < 1) || (rx % 2 > 0 && ry % 2 > 0)) ?
                coloreaza(3, rx, ry, local_win):
                coloreaza(4, rx, ry, local_win);
            break;
        case KEY_RIGHT:
        // analog ca la key up
            ((rx % 2 < 1 && ry % 2 < 1) || (rx % 2 > 0 && ry % 2 > 0)) ?
                coloreaza(1, rx, ry, local_win):
                    coloreaza(2, rx, ry, local_win);
            if (matr[rx][ry] == 3)
                coloreaza(6, rx, ry, local_win);
            else if (matr[rx][ry] == 4)
                coloreaza(5, rx, ry, local_win);
            ry++;
            ry = (ry > 9) ? 0 : ry;
            ((rx % 2 < 1 && ry % 2 < 1) || (rx % 2 > 0 && ry % 2 > 0)) ?
                coloreaza(3, rx, ry, local_win):
                    coloreaza(4, rx, ry, local_win);
            break;
        case 10:
            // daca a nimerit o patratica corecta afiseaza ca e bine si 
            // daca era necolorata cu verde o coloreaza
            // si creste contorul nimerite si ma mai lasa sa dau o data
            if (matr[rx][ry] == 1 || matr[rx][ry] == 3) {
                wattron(local_win, COLOR_PAIR(3));
                wattroff(local_win, COLOR_PAIR(3));
                wattroff(local_win, COLOR_PAIR(1));
                wattroff(local_win, COLOR_PAIR(2));
                wattroff(local_win, COLOR_PAIR(4));
                wattroff(local_win, COLOR_PAIR(5));
                wattroff(local_win, COLOR_PAIR(6));
                wattroff(local_win, COLOR_PAIR(7));

                mvwprintw(local_win, 2, 40,
                    " Corect bossache                   ");
                coloreaza(6, rx, ry, local_win);
                * nimerite = (matr[rx][ry] == 1) ?
                    * nimerite + 1 : * nimerite + 0;
                matr[rx][ry] = 3;
                wrefresh(local_win);
                ok = 1;
            } else {
                // daca era gresita o coloreaza cu rosu
                wattron(local_win, COLOR_PAIR(3));
                wattroff(local_win, COLOR_PAIR(3));
                wattroff(local_win, COLOR_PAIR(1));
                wattroff(local_win, COLOR_PAIR(2));
                wattroff(local_win, COLOR_PAIR(4));
                wattroff(local_win, COLOR_PAIR(5));
                wattroff(local_win, COLOR_PAIR(6));
                wattroff(local_win, COLOR_PAIR(7));
                mvwprintw(local_win, 2, 40,
                    "Alta data bosache                   ");
                coloreaza(5, rx, ry, local_win);
                matr[rx][ry] = 4;
                wrefresh(local_win);
                ok = 0;
            }
            if ( * nimerite == 20 ) {
                // daca au fost toate nimerite anunta finalul jocului
                // si castigatorul si afiseaza contorul
                wattron(local_win, COLOR_PAIR(3));
                wattroff(local_win, COLOR_PAIR(3));
                wattroff(local_win, COLOR_PAIR(1));
                wattroff(local_win, COLOR_PAIR(2));
                wattroff(local_win, COLOR_PAIR(4));
                wattroff(local_win, COLOR_PAIR(5));
                wattroff(local_win, COLOR_PAIR(6));
                wattroff(local_win, COLOR_PAIR(7));

                mvwprintw(local_win, 2, 40,
                    " Ai castigat bossuleeeeeeee!!       ");
                verificare_pierzator(eu, local_win, nave_sparte[0], 1, 0);
                jocgata = 1;

                break;
            }
            while (!ok) {
                //alege un punct random si verifica daca a mai fost incercat
                // si daca da alege altul
                rx2 = rand() % 10;
                ry2 = rand() % 10;
                while (eu[rx2][ry2] != 1 && eu[rx2][ry2] != 0) {
                    rx2 = rand() % 10;
                    ry2 = rand() % 10;
                }
                napms(3000); // asteptam 3 secunde pt a da senzatia de real
                // si ca era task

                if (eu[rx2][ry2] == 1 || eu[rx2][ry2] == 3) {
                    wattron(local_win, COLOR_PAIR(3));
                    wattroff(local_win, COLOR_PAIR(3));
                    wattroff(local_win, COLOR_PAIR(1));
                    wattroff(local_win, COLOR_PAIR(2));
                    wattroff(local_win, COLOR_PAIR(4));
                    wattroff(local_win, COLOR_PAIR(5));
                    wattroff(local_win, COLOR_PAIR(6));
                    wattroff(local_win, COLOR_PAIR(7));
                    // daca era corect o coloreaza MAGENTA sin verde si 
                    // creste contorul si mai da o data
                    mvwprintw(local_win, 2, 40,
                        " Corect bossache calculator       ");
                    * nimeritecalc = (eu[rx2][ry2] == 1) ?
                        * nimeritecalc + 1 : * nimeritecalc + 0;
                    coloreaza_eu(7, rx2, ry2, local_win);
                    eu[rx2][ry2] = 3;
                    wrefresh(local_win);
                    ok = 0;
                } else {
                    // daca era gresita o face rosie
                    wattron(local_win, COLOR_PAIR(3));
                    wattroff(local_win, COLOR_PAIR(3));
                    wattroff(local_win, COLOR_PAIR(1));
                    wattroff(local_win, COLOR_PAIR(2));
                    wattroff(local_win, COLOR_PAIR(4));
                    wattroff(local_win, COLOR_PAIR(5));
                    wattroff(local_win, COLOR_PAIR(6));
                    wattroff(local_win, COLOR_PAIR(7));
                    mvwprintw(local_win, 2, 40,
                        "Alta data bosache calculator          ");
                    coloreaza_eu(5, rx2, ry2, local_win);
                    eu[rx2][ry2] = 4;
                    wrefresh(local_win);
                    ok = 1;
                }
            }
            if ( * nimeritecalc == 20 ) {
                // daca calculatorul a castigat afiseaza mesajul
                wattroff(local_win, COLOR_PAIR(3));
                wattroff(local_win, COLOR_PAIR(1));
                wattroff(local_win, COLOR_PAIR(2));
                wattroff(local_win, COLOR_PAIR(4));
                wattroff(local_win, COLOR_PAIR(5));
                wattroff(local_win, COLOR_PAIR(6));
                wattroff(local_win, COLOR_PAIR(7));

                mvwprintw(local_win, 2, 40,
                 " Ai castigat bossuleeeeeeee calculator!!          ");
                verificare_pierzator(matr, local_win, nave_sparte[1], 1, 45);
                jocgata = 1;
                break;
            }
            break;
        case 'd':
            // se bazeaza pe principiul de selectare random a calculatorului
            // de data asta extins si la utilizator
            // alege un punct random neselectat si daca e corect il coloreaza
            // si mai da o data si daca nu il coloreaza cu rosu
            // totul intr un for pt a realiza 10 mutari
            for (int i = 0; i < 10; ++i) {
                while (!ok) {
                    rx = rand() % 10;
                    ry = rand() % 10;
                    while (matr[rx][ry] != 1 && matr[rx][ry] != 0
                        && matr[rx][ry] != 2) {
                        rx = rand() % 10;
                        ry = rand() % 10;
                    }
                    if (matr[rx][ry] == 1 || matr[rx][ry] == 3) {
                        wattron(local_win, COLOR_PAIR(6));
                        * nimerite = (matr[rx][ry] == 1) ? * nimerite + 1
                            : * nimerite + 0;
                        coloreaza(6, rx, ry, local_win);
                        matr[rx][ry] = 3;
                        ok = 0;
                    } else {
                        wattron(local_win, COLOR_PAIR(5));
                        coloreaza(5, rx, ry, local_win);
                        matr[rx][ry] = 4;
                        wrefresh(local_win);
                        ok = 1;
                    }
                }
                if ( * nimerite == 20 ) {
                    wattron(local_win, COLOR_PAIR(3));
                    wattroff(local_win, COLOR_PAIR(3));
                    wattroff(local_win, COLOR_PAIR(1));
                    wattroff(local_win, COLOR_PAIR(2));
                    wattroff(local_win, COLOR_PAIR(4));
                    wattroff(local_win, COLOR_PAIR(5));
                    wattroff(local_win, COLOR_PAIR(6));
                    wattroff(local_win, COLOR_PAIR(7));
                    mvwprintw(local_win, 2,
                        40, " Ai castigat bossuleeeeeeee!!           ");
                    jocgata = 1;

                    verificare_pierzator(eu, local_win, nave_sparte[0], 1, 0);
                    break;
                }
                ok = 0;
                while (!ok) {
                    rx2 = rand() % 10;
                    ry2 = rand() % 10;
                    while (eu[rx2][ry2] != 1 && eu[rx2][ry2] != 0) {
                        rx2 = rand() % 10;
                        ry2 = rand() % 10;
                    }
                    if (eu[rx2][ry2] == 1 || eu[rx2][ry2] == 3) {
                        wattron(local_win, COLOR_PAIR(3));
                    * nimeritecalc = (eu[rx2][ry2] == 1) ? * nimeritecalc + 1
                                 : * nimeritecalc + 0;
                        coloreaza_eu(7, rx2, ry2, local_win);
                        eu[rx2][ry2] = 3;
                        wrefresh(local_win);
                        ok = 0;
                    } else {
                        wattron(local_win, COLOR_PAIR(3));
                        coloreaza_eu(5, rx2, ry2, local_win);
                        eu[rx2][ry2] = 4;
                        wrefresh(local_win);
                        ok = 1;
                    }
                }
                if ( * nimeritecalc == 20 ) {
                    wattron(local_win, COLOR_PAIR(3));
                    wattroff(local_win, COLOR_PAIR(3));
                    wattroff(local_win, COLOR_PAIR(1));
                    wattroff(local_win, COLOR_PAIR(2));
                    wattroff(local_win, COLOR_PAIR(4));
                    wattroff(local_win, COLOR_PAIR(5));
                    wattroff(local_win, COLOR_PAIR(6));
                    wattroff(local_win, COLOR_PAIR(7));
                    mvwprintw(local_win, 2, 40,
                        " Ai castigat bossuleeeeeeee calculator!!          ");
                    verificare_pierzator(matr,
                        local_win, nave_sparte[1], 1, 45);
                    jocgata = 1;
                    break;
                }
                ok = 0;
            }
            break;

        case 'r':
            //copiaza matricea calculatorului
            // retine nr de nave sparte pt contor
            // randomizeaza matricea calculatorului
            // analog la matricea mea
            copie_matr(matr, copie);
            nave_sparte[1] = verificare_pierzator(copie,
                local_win, nave_sparte[1], 0, 0);
            randomizare(matr, local_win);
            copie_matr(eu, copie);
            nave_sparte[0] = verificare_pierzator(copie,
                 local_win, nave_sparte[0], 0, 0);
            randomizare(eu, local_win);
            init_joc(local_win, eu, matr);
            wattron(local_win, COLOR_PAIR(4));

            break;

        case 'q':
            // se iese din joc si se retine in resume daca 
            // jocul poate fi reluat sau nu
            *resume = !jocgata;
            wrefresh(local_win);
            wclear(local_win);
            endwin();

            return 0;
        }
    }

    return local_win;
}
// int main(int argc, char *argv[])
// {
//     int matr[10][10];
//     int eu[10][10];
//     initscr();
//     if (has_colors() == false)
//     {
//         endwin();
//         printf("No color support!\n");
//         return -1;
//     }
//     start_color();
//     // mvprintw(53,103,"skrrr\n");
//     create_board(0, 0, 120, 50,0,matr,eu);
//     napms(150000);

//     endwin();

//     return 0;
// }
