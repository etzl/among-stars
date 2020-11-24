#include <curses.h>
#include "objects.h"


bool quit = false;
constexpr float _timeout = 0.25;


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
    // curses initializations
    initscr();
    cbreak();
    nonl();
    keypad(stdscr, TRUE);
    noecho();
    timeout(_timeout);
}

void input()
{
    int c = getch();
}
