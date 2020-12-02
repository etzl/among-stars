#include <curses.h>
#include <string>
#include "objects.h"
#include "gamemanager.h"



constexpr float _timeout = 0.25;

// player starting positions
constexpr int _Player_initialx = 20;
constexpr int _Player_initialy = 30;


void init();
bool input();
void update();
void draw();
void showmessage(char *msg);    /* show a message for specified time */
int finish();

int main()
{
    init();

    while (!input()) {
        update();
        draw();
    }
    return finish();
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

    Game_manager::player = Player{_Player_initialy, _Player_initialx};
}
int finish()
{
    int err = 0;
    err = endwin();
    return err;
}

bool input()
{
    int c = getch();
    Dir yplayerdir = Dir::none;
    Dir xplyaerdir = Dir::none;

    switch (c) {
        case 'w':
        case KEY_UP:
            yplayerdir = Dir::up;
            break;
        case 's':
        case KEY_DOWN:
            yplayerdir = Dir::down;
            break;
        case 'a':
        case KEY_LEFT:
            xplyaerdir = Dir::left;
            break;
        case 'd':
        case KEY_RIGHT:
            xplyaerdir = Dir::right;
            break;
        case ' ':
        case '\n':
            Game_manager::player.shoot();
            break;
        case 'q':
            return 1;
        default:
            showmessage("Wrong input!");
            break;
    }
    Game_manager::player.move(xplyaerdir, yplayerdir);
    return 0;
}
