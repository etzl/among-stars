#include <curses.h>
#include <panel.h>
#include <menu.h>

#include <algorithm>
#include <thread>
#include <mutex>
#include <chrono>
#include <cstring>
#include <regex>

#include "objects.h"
#include "gamemanager.h"
#include "bullet.h"



constexpr int _timeout = 50;
constexpr std::chrono::seconds _Showmessage_timer(1);



PANEL *stdpnl, *pointspnl, *msgpnl;
WINDOW *pointswin, *menuwin, *msgwin, *descwin;
MENU* mainmenu;
std::vector<ITEM*> m_items;


void init();
void input();
void update();
void draw();
void showmessage(const char*);    /* show a message for specified time */
void finish();
void showmenu();
void print_per_line(WINDOW*, const char*);


int main()
{
    init();

    while (true) {
        input();
        update();
        draw();
    }
}

void init()
{
    // curses initializations
    initscr();
    cbreak();
    nl();
    noecho();
    curs_set(0);
    timeout(_timeout);
    keypad(stdscr, TRUE);

    init_pair(1, COLOR_GREEN, 0);
    init_pair(2, COLOR_RED, 0);
    init_pair(3, COLOR_WHITE, 0);
    init_pair(4, COLOR_BLUE, 0);

    // windows
    pointswin = newwin(3, 0, 0, COLS-5);
    msgwin = newwin(0, 0, LINES-5, COLS-10);
    int m_nlines = 12, m_ncols = 50, m_begy = (LINES-m_nlines)/2,
     m_begx = (COLS-m_ncols)/2;
    menuwin = newwin(m_nlines, m_ncols, m_begy, m_begx);

    keypad(menuwin, TRUE);

    int m_dernlines = m_nlines-3, m_derncols = 20, m_derbegy = 2, m_derbegx = 4;
    int m_descols = 30;
    descwin = derwin(menuwin, m_dernlines, m_descols, 1, m_ncols-m_descols-1);

    wattron(msgwin, COLOR_PAIR(2));

    // items
    m_items.reserve(5);
    m_items.push_back(new_item("Resume", "Continue where you left"));
    m_items.push_back(new_item("New Game", nullptr));
    m_items.push_back(new_item("Help", "How to play"));
    m_items.push_back(new_item("Exit", nullptr));
    m_items.push_back(nullptr);

    // if (!save)
    item_opts_off(m_items[0], O_SELECTABLE);

    mainmenu = new_menu(m_items.data());

    // menu customizations
    set_menu_win(mainmenu, menuwin);
    set_menu_sub(mainmenu, derwin(menuwin, m_dernlines, m_derncols, m_derbegy,
        m_derbegx));
    set_menu_format(mainmenu, m_dernlines-2, 1);
    menu_opts_off(mainmenu, O_SHOWDESC);
    set_menu_mark(mainmenu, "> ");

    showmenu();

    stdpnl = new_panel(stdscr);
    pointspnl = new_panel(pointswin);
    msgpnl = new_panel(msgwin);

    update_panels();
    doupdate();

    Game_manager::player = Player{static_cast<float>(LINES)/2,
     static_cast<float>(COLS)/2};

    for (int line=1; line<=3; ++line)
        for (int cols=5; cols<=(COLS-5); cols+=6)
            Game_manager::enemies.emplace_back(line, cols);
}
void finish()
{
    unpost_menu(mainmenu);
    free_menu(mainmenu);
    for (auto& it: m_items)
        free_item(it);
    // delete associate panels
    del_panel(pointspnl);
    del_panel(stdpnl);
    del_panel(msgpnl);
    // delete windows
    delwin(pointswin);
    delwin(menuwin);
    delwin(msgwin);
    delwin(descwin);
    endwin();
    exit(0);
}

void input()
{
    int c = getch();
    if (c == ERR)
        return;
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
            Game_manager::bullets.push_back(Game_manager::player.shoot());
            break;
        case 'q':   // a shortcut for quiting
            finish();
        case 27:    // <Esc>
            showmenu();
            break;
        default:
            showmessage("Wrong input!");
            break;
    }
    Game_manager::player.move(xplyaerdir, yplayerdir);
}

void draw()
{
    erase();

    Game_manager::player.draw(stdscr);

    std::for_each(Game_manager::enemies.begin(), Game_manager::enemies.end(),
    [](Enemy& enemy) { enemy.draw(stdscr); });

    std::for_each(Game_manager::bullets.begin(), Game_manager::bullets.end(),
    [](Bullet& b) { b.draw(stdscr); });

    update_panels();
    doupdate();
}

std::mutex access_curses;
/* This function should not be called directly but from 'showmessage' function */
void task_showmessage(const char* msg)
{
    std::scoped_lock lock {access_curses};

    print_per_line(msgwin, msg);

    show_panel(msgpnl);
    update_panels();
    doupdate();

    std::this_thread::sleep_for(_Showmessage_timer);

    hide_panel(msgpnl);
    update_panels();
    doupdate();
}
void showmessage(const char* msg)
{
    std::thread task {task_showmessage, msg};
    task.detach();
}

void update()
{
    // Game_manager::generate_enemies();
    // Game_manager::move_enemies();
    Game_manager::update();
}

void showmenu_desc()
{
    print_per_line(descwin, item_description(current_item(mainmenu)));

    static const int maxcols = getmaxx(descwin);
    mvwaddstr(descwin, 0, (maxcols-std::strlen("Description"))/2, "Description");
    wrefresh(descwin);
}

void showmenu()
{
    // update background!
    erase();
    refresh();

    box(menuwin, 0, 0);
    mvwaddstr(menuwin, 0, 2, "Menu!");
    post_menu(mainmenu);
    showmenu_desc();
    wrefresh(menuwin);

    int ch = 0;
    while (ch != 27) {
        ch = wgetch(menuwin);
        switch (ch) {
            case KEY_DOWN:
            case 's':
                menu_driver(mainmenu, REQ_DOWN_ITEM);
                showmenu_desc();
                break;
            case KEY_UP:
            case 'w':
                menu_driver(mainmenu, REQ_UP_ITEM);
                showmenu_desc();
                break;
            case '\n':
                ITEM* cur_item = current_item(mainmenu);
                if (item_opts(cur_item) & O_SELECTABLE) {
                    std::string iname {item_name(cur_item)};
                    if (iname == "Exit")
                        finish();
                    else if (iname == "New Game")
                        ch = 27;
                }
                break;
        }
        wrefresh(menuwin);
    }

    unpost_menu(mainmenu);
    wrefresh(menuwin);
}

void print_per_line(WINDOW* place, const char* msg)
{
    using namespace std;

    werase(place);
    box(place, 0, 0);

    int cur_line = 1;
    wmove(place, cur_line, 1);
    wrefresh(place);

    if (msg == nullptr)
        return;
    string_view desc {msg};
    // use only white-space to seperate words
    static const regex pattern {R"((\S+\s*))"};
    size_t count = 0;
    string_view::const_iterator remain = desc.cbegin();

    const int maxx = getmaxx(place);
    const int allowed_m = maxx-2;   // occupied by box around the window

    if (allowed_m < desc.size()) {
        for (cregex_iterator iter {desc.cbegin(), desc.cend(), pattern}; ; ++iter) {
            // reached the end of description before reaching end of line
            if (iter == cregex_iterator{}) {
                for (auto p = remain; p != remain+count; ++p)
                    waddch(place, *p);
                break;
            }

            int tmp = (*iter)[1].length();
            count += tmp;

            if (count >= allowed_m) {
                if (count != allowed_m)
                    count -= tmp;
                for (auto p = remain; p != remain+count; ++p)
                    waddch(place, *p);
                wmove(place, ++cur_line, 1);

                remain += count;
                count = (count == allowed_m) ? 0 : tmp;
            }
        }
    }
    else wprintw(descwin, "%s", desc.data());

    wrefresh(place);
}
