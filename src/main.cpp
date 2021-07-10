// #define _DEBUG

#include <curses.h>
#include <panel.h>
#include <menu.h>
#include <algorithm>
#include <thread>
#include <mutex>
#include <atomic>
#include <chrono>
#include <cstring>
#include <string>
#include <regex>
#include <chrono>
#include <cassert>

#include "objects.h"
#include "gamemanager.h"
#include "bullet.h"





PANEL *stdpnl, *pointspnl, *msgpnl;
WINDOW *pointswin, *menuwin, *msgwin, *descwin;
MENU* mainmenu;
std::vector<ITEM*> m_items;
std::mutex access_curses;
std::atomic_bool thr_run = false;
bool menu = false;


void init(bool);
void input();
void update();
void draw();
void showmessage(const char*);    /* show a message for specified time */
void finish();
void showmenu();
void text_buffer(WINDOW*, const char*);


void checkargs(int count, char* argv[], bool& rwm)
{
    for (int i=0; i!=count; ++i) {
        if (std::strcmp(argv[i], "--no-menu") == 0)
            rwm = true;
    }
}

int main(int argc, char* argv[])
{
    bool runwithoutmenu = false;
    if (0 < argc)
        checkargs(argc, argv, runwithoutmenu);

    init(runwithoutmenu);

    while (true) {
        auto tp1 = std::chrono::steady_clock::now();

        input();
        update();
        draw();

        auto tp2 = std::chrono::steady_clock::now();
        Game_manager::deltatime = std::chrono::duration<float>(tp2-tp1).count();
        if (menu) { // dont include the time we spent in the menu, also give player 1 frame to think about
            menu = false;
            Game_manager::deltatime = 0;
        }
    }
}

void init(bool rwm)
{
    // curses initializations
    initscr();
    cbreak();
    nl();
    noecho();
    curs_set(0);
    timeout(_timeoutms.count());
    keypad(stdscr, TRUE);

    init_pair(1, COLOR_GREEN, 0);
    init_pair(2, COLOR_RED, 0);
    init_pair(3, COLOR_WHITE, 0);
    init_pair(4, COLOR_BLUE, 0);

    // windows
    pointswin = newwin(3, 0, 0, COLS-5);
    msgwin = newwin(0, 0, LINES-4, COLS-10);
    int m_nlines = 12, m_ncols = 50, m_begy = (LINES-m_nlines)/2,
     m_begx = (COLS-m_ncols)/2;
    menuwin = newwin(m_nlines, m_ncols, m_begy, m_begx);

    keypad(menuwin, TRUE);

    int m_dernlines = m_nlines-3, m_derncols = 20, m_derbegy = 2, m_derbegx = 4;
    int m_descols = 30;
    descwin = derwin(menuwin, m_dernlines, m_descols, 1, m_ncols-m_descols-1);

    // items
    m_items.reserve(5);
    m_items.push_back(new_item("Resume", "Continue where you left"));
    m_items.push_back(new_item("New Game", "Start a fresh game"));
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

    stdpnl = new_panel(stdscr);
    pointspnl = new_panel(pointswin);
    msgpnl = new_panel(msgwin);

    update_panels();
    doupdate();

    Game_manager::player = Player{static_cast<float>(_Player_initial_y),
     static_cast<float>(_Player_initial_x)};

#ifndef _DEBUG
    if (!rwm)
        showmenu();
#endif

    item_opts_on(m_items[0], O_SELECTABLE);
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

    switch (c) {
        case 'w':
        case KEY_UP:
            Game_manager::player.move(Dir::up);
            break;
        case 's':
        case KEY_DOWN:
            Game_manager::player.move(Dir::down);
            break;
        case 'a':
        case KEY_LEFT:
            Game_manager::player.move(Dir::left);
            break;
        case 'd':
        case KEY_RIGHT:
            Game_manager::player.move(Dir::right);
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
}

void draw()
{
    std::scoped_lock lock {access_curses};

    erase();

    Game_manager::player.draw(stdscr);

    std::for_each(Game_manager::enemies.begin(), Game_manager::enemies.end(),
    [](Enemy& enemy) { enemy.draw(stdscr); });

    std::for_each(Game_manager::bullets.begin(), Game_manager::bullets.end(),
    [](Bullet& b) { b.draw(stdscr); });

    update_panels();
    doupdate();
}

/* This function used only by showmessage(), **DO NOT CALL IT DIRECTLY!** */
void task_showmessage(const char* msg)
{
    std::unique_lock lck {access_curses};

    show_panel(msgpnl);
    text_buffer(msgwin, msg);

    update_panels();
    doupdate();

    lck.unlock();
    std::this_thread::sleep_for(_Showmessage_timer);
    lck.lock();

    hide_panel(msgpnl);
    update_panels();
    doupdate();

    thr_run = false;
}
void showmessage(const char* msg)
{
    if (thr_run)
        return;

    thr_run = true;
    std::thread task {task_showmessage, msg};
    task.detach();
}

void update()
{
    Game_manager::generate_enemies();
    Game_manager::move_enemies();
    Game_manager::shoot();
    Game_manager::update();
}

void showmenu_desc()
{
    text_buffer(descwin, item_description(current_item(mainmenu)));

    static const int maxcols = getmaxx(descwin);
    mvwaddstr(descwin, 0, (maxcols-std::strlen("Description"))/2, "Description");
    wrefresh(descwin);
}

void showmenu()
{
    menu = true;
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
                    else if (iname == "New Game") {
                        if (item_opts(m_items[0]) & O_SELECTABLE) {
                            Game_manager::restart();
                        }
                        ch = 27;
                    }
                    else if (iname == "Resume")
                        ch = 27;
                    else if (iname == "Help") {
                        // show help
                    }
                }
                break;
        }
        wrefresh(menuwin);
    }

    unpost_menu(mainmenu);
    wrefresh(menuwin);
}

void text_buffer(WINDOW* place, const char* msg)
{
    using namespace std;
    if (msg == nullptr)
        return;

    // design (prepare) the window
    auto design_w = [](WINDOW* win, const char* title) {
        int maxx = getmaxx(win);
        werase(win);
        box(win, 0, 0);
        mvwaddstr(win, 0, (maxx-strlen(title))/2, title);
    };
    // returns true if the character is not an attribute character
    auto not_attr = [](const char x) {
        constexpr string_view attrib = "*\n\b";
        return attrib.find(x) == string_view::npos;
    };

    static const regex pattern {R"((\S+\s*))"};
    string_view desc {msg};
    int bufl = 0, bufc = 0, lsize = 0, winattrs = 0, prev = 0;

    // do not use these two variables; use 'maxc' and 'maxl' instead
    int maxx, maxy;
    getmaxyx(place, maxy, maxx);
    const int maxc = maxx - 2, maxl = maxy - 2; // sizes which we can actually use
    int m_line = maxl;

    design_w(place, "");

    vector<chtype> buffer; buffer.reserve(m_line*maxc);

    /* write white-space until the end of current line
     * column = position to start in the line; passing variable 'bufc' causes to write
     * white-space to the remaining characters in the buffer
     */
    auto newline_buf = [&](int column) {
        bufc = 0; bufl++;
        int size = maxc - column - 1;
        while (size-- > 0) {
            buffer.push_back(32);
        }
    };

    bool bflag = false;

    // process text
    for (cregex_iterator iter {desc.cbegin(), desc.cend(), pattern}; iter!=cregex_iterator{}; ++iter) {
        string word = iter->str(1);

        int wsize = count_if(word.begin(), word.end(), not_attr);
        lsize += wsize;
        if (lsize > maxc && wsize < maxc && word.front() != '\n') {
            newline_buf(bufc);
            lsize = wsize;
        }

        for (auto ch=word.begin(); ch!=word.end(); ++ch) {
            if (*ch == '<');
                // winattrs |= COLOR_PAIR(5);
            else if (*ch == '*') {
                if (winattrs & A_BOLD) {
                    if (prev == '*')
                        winattrs ^= A_ITALIC;
                    winattrs ^= A_BOLD;
                }
                else
                    winattrs |= A_BOLD;

                prev = *ch;
                continue;
            }
            else if (*ch == '\n') {
                newline_buf(bufc);
                auto next = ch + 1;
                if (regex_match(next, word.end(), regex{R"(\s+)"})) {
                    lsize = 0;
                    break;
                }
                else {
                    lsize = count_if(next, word.end(), not_attr);
                }
                continue;
            }
            else if (*ch == '\b') {
                /* NOTE: this logic assumes that you put \b signs before and
                 * after each word. If you do otherwise (e.g. put them between
                 * each word) you might see strange results
                 */
                if (bflag) {
                    // another pair of \b: do nothing
                    bflag = false;
                    continue;
                }

                lsize -= count_if(ch+1, word.end(), not_attr);

                // find next \b
                for (++iter; iter!=cregex_iterator{}; iter++) {
                    string_view tmp = iter->str(1);
                    word += tmp;
                    if (tmp.find('\b') != string::npos) {
                        bflag = true;
                        break;
                    }
                }
                if (!bflag) // This is probably not what we wanted!
                    throw "another \\b character not found";

                // count new length
                wsize = count_if(ch+1, word.end(), not_attr);
                lsize += wsize;

                assert(wsize < maxc);
                if (lsize > maxc && wsize < maxc)
                    newline_buf(bufc);

                continue;
            }

            // make room for new characters
            if (bufc == maxc)
                newline_buf(bufc);
            if (bufl == m_line) {
                m_line += maxl;
                buffer.reserve(m_line*maxc);
            }

            // finally write to buffer
            buffer.push_back(*ch | winattrs);
            bufc++;
            prev = *ch;

            if (*ch == '>');
                // winattrs ^= COLOR_PAIR(5);
        }
    }

    // print buffer
    bufl=0; // mark the begging of the buffer - we will start printing from here
    int winline = 1; // track current line in the window (starting from 1)
    const int maxbufl = buffer.size() / maxc;

    for (int line = bufl; line < maxbufl; ++line) {
        wmove(place, ++winline, 1);

        // window lines exceeded - go to interactive mode
        if (winline > maxl) {
            get_input:
                int ch = wgetch(place);
                switch (ch) {
                    case 'k':
                    case KEY_UP:
                        if (bufl != 0) {
                            line = --bufl;
                            break;
                        }
                        goto get_input;
                    case 'j':
                    case KEY_DOWN:
                        if (bufl != maxbufl) {
                            line = ++bufl;
                            break;
                        }
                        goto get_input;
                    case 'q':
                    default:
                        goto get_input;
                }


            winline = 1;
            wmove(place, winline, 1);
        }

        for (bufc = 0; bufc < maxc; bufc++) {
            waddch(place, buffer[line*maxc + bufc]);
        }
    }

    wrefresh(place);
}
