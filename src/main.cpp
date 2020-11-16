#include <curses.h>
#include "objects.h"


bool quit = false;


void init();
void input();
void update();
void draw();

int main()
{
    init();

    while (!quit) {
        input();
        update();
        draw();
    }
    endwin();

    return 0;
}

void init()
{
    initscr();
    cbreak();
    nonl();
    keypad(stdscr, TRUE);
    noecho();
}
