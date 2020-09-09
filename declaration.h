#ifndef __createboard__
#define __createboard__
WINDOW* create_board(int x, int y, int width, int height,int *resume,int matr[10][10],int eu[10][10],int nave_sparte[2] ,int *nimerite, int *nimeritecalc);
#endif 

#ifndef __init_matr__
#define __init_matr__
void init_matr(int matr[10][10]);
#endif 

#ifndef __print_matr__
#define __print_matr__
void print_matr(int matr[10][10]);
#endif 

#ifndef __citire_fisier__
#define __citire_fisier__
int citire_fisier(int eu[10][10],char a[30]);
#endif 

#ifndef __verificare_fisier__
#define __verificare_fisier__
int verificare_fisier(char a[30]);
#endif 

#ifndef __coloreaza__
#define __coloreaza__
void coloreaza(int culoare, int x, int y, WINDOW *local_win);
#endif 

#ifndef __coloreaza__
#define __coloreaza__
void coloreaza(int culoare, int x, int y, WINDOW *local_win);
#endif 

#ifndef __coloreaza_eu__
#define __coloreaza_eu__
void coloreaza_eu(int culoare, int x, int y, WINDOW *local_win);
#endif  

#ifndef __scriere_matrice__
#define __scriere_matrice__
void scriere_matrice(int matr[10][10], int dir, int x, int y, int nrpatr);
#endif 

#ifndef __parcurgere_matrice__
#define __parcurgere_matrice__
int parcurgere_matrice(int matr[10][10], int dir, int x, int y, int nrpatr);
#endif 

#ifndef __parcurgere_nava__
#define __parcurgere_nava__
int parcurgere_nava(int matr[10][10], int x, int y, int dir);
#endif 

#ifndef __parcurgere_rand__
#define __parcurgere_rand__
int parcurgere_rand(int matr[10][10], int x, int y, int dir);
#endif 

#ifndef __plasare_random__
#define __plasare_random__
void plasare_random(int matr[10][10],int nave[13], int nrnave);
#endif 

#ifndef __randomizare__
#define __randomizare__
void randomizare(int matr[10][10], WINDOW* local_win);
#endif 

#ifndef __verificare_pierzator__
#define __verificare_pierzator__
int verificare_pierzator(int matr[10][10], WINDOW* local_win, int nave_sparte, int final, int loc);
#endif 

#ifndef __generare_matrice__
#define __generare_matrice__
void generare_matrice(int matr[10][10]);
#endif 

#ifndef __init_joc__
#define __init_joc__
void init_joc(WINDOW *local_win, int eu[10][10],int matr[10][10]);
#endif 

#ifndef __menu__
#define __menu__
WINDOW *menu(int x, int y, int width, int height,int* resume ,int matr[10][10],int eu[10][10],int argc, char **argv );
#endif 

#ifndef __coloreaza_centru__
#define __coloreaza_centru__
void coloreaza_centru(int culoare, int x, int y, WINDOW *local_win);
#endif 

#ifndef __afis_tabla__
#define __afis_tabla__
void afis_tabla(int matr[10][10], WINDOW *local_win);
#endif 


#ifndef __alegere__
#define __alegere__
WINDOW *alegere(int x, int y, int width, int height,int matr[10][10],int argc, char **argv);
#endif 


#ifndef __draw_cat_step__
#define __draw_cat_step__
void draw_cat_step(int x, int y, char *tail ,const  char nyan_cat[4][12]);
#endif


#ifndef __flames__
#define __flames__
void flames(void);
#endif

