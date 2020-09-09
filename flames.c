
#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include"declaration.h"

void flames(void);

// int main(int argc, char** argv)
// 	{
// 	initscr();
// 	start_color();

// 	flames();

// 	endwin();
// 	return 0;
// 	}

void flames(void)
	{
	int width, height, size, *b, i;
	const char characters[] = {' ', '.', ':', '^', '*', 'x', 's', 'S', '#', '$'};
	
	getmaxyx(stdscr,height,width);
	size=width*height;
	init_pair(1,0,0);
	init_pair(2,1,0);
	init_pair(3,3,0);
	init_pair(4,4,0);
	clear();
	
	b=calloc((size+width+1),sizeof(int));
	nodelay(stdscr,TRUE);
	srand(time(NULL));
	 const char nyan_cat[4][12] = {
    ",------,   \n",
    "|   /\\_/\\\n",
    "|__( ^ .^) \n",
    "\"\"  \"\" \n"};

     init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_YELLOW, COLOR_BLACK);
    init_pair(3, COLOR_CYAN, COLOR_BLACK);
    init_pair(4, COLOR_GREEN, COLOR_BLACK);
    init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
    noecho(); // disable echoing of characters on the screen
    curs_set(0); // hide the default screen cursor.

    
    /*  while we have room, we will animate our cat crossing... */
    int c =0;
    char *tail = (char*)malloc(width-8);
    while (c < width-8 ) 
		{
		for (i =0; i < width/9; i++)
			b[(int)(((float)rand()/(float)RAND_MAX)*width+width*(height-1))]=65;
		for (i=0; i < size; i++)
			{
			b[i]=(b[i]+b[i+1]+b[i+width]+b[i+width+1])/4;
			if (b[i] > 15)
				attrset(COLOR_PAIR(4)|A_BOLD);
			else if (b[i] > 9)
				attrset(COLOR_PAIR(3)|A_BOLD);
			else if (b[i] > 4)
				attrset(COLOR_PAIR(2)|A_BOLD);
			else
				attrset(COLOR_PAIR(1)|A_BOLD);
			if (i < size-1)
				{
				move(i/width, i%width);
				if (b[i] > 9)
					addch(characters[9]);
				else
					addch(characters[b[i]]);

				}
			}

		draw_cat_step(c, 5 , tail, nyan_cat);
        mvprintw(25,47,"BATTLESHIP");

		refresh();
		timeout(30);
        c++;

        napms(50);
		}
	
	
	free(b);
	free(tail);

	return;
	}


