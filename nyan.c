#include <stdlib.h>
#include <ncurses.h>
#include"declaration.h"



void draw_cat_step(int x, int y, char *tail ,const  char nyan_cat[4][12]) {
    int i, w=0;

    for(i=0; i<x; i++) {
        *(tail+i) = '=';
    }

    for(w=0; w<4; w++) {
        attron(COLOR_PAIR(w+1));
        mvprintw(y+w, 0, tail);
        attron(COLOR_PAIR(5));
        mvprintw(y+w, x, nyan_cat[w]);
    }
}

// void nyan(){
//  const char nyan_cat[4][12] = {
//     ",------,   \n",
//     "|   /\\_/\\\n",
//     "|__( ^ .^) \n",
//     "\"\"  \"\" \n"};

//      init_pair(1, COLOR_RED, COLOR_BLACK);
//     init_pair(2, COLOR_YELLOW, COLOR_BLACK);
//     init_pair(3, COLOR_CYAN, COLOR_BLACK);
//     init_pair(4, COLOR_GREEN, COLOR_BLACK);
//     init_pair(5, COLOR_MAGENTA, COLOR_BLACK);

//     int width = 0;
//     getmaxyx(stdscr, height, width);
//     int c =0;
//     char *tail = (char*)malloc(width-8);
//     while (c < width-8 ) {
//         refresh();
//         c++;
//         draw_cat_step(c, 5 , tail, nyan_cat);
//         napms(50);
//     }
//     endwin();
//     free(tail);
// }

// int main(int argc, char * argv) {
//     initscr();
   
//     start_color();
//     nyan();

//     return 0;
// }
