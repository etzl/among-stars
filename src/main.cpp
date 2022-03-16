// #define _DEBUG

#include <curses.h>
#include <panel.h>
#include <menu.h>
#include <algorithm>
#include <thread>
#include <mutex>
#include <atomic>
#include <cstring>
#include <string>
#include <regex>
#include <string_view>
#include <cmath>
#include <limits>
#include <utility>
#include <iostream>

#include "objects.hpp"
#include "gamemanager.hpp"
#include "bullet.hpp"
#include "stats.hpp"

/* ====================CONSTANTS==================== */
constexpr std::string_view Help_description {"Welcome to the ***Among Stars***.\nCheckout the latest version of the game and technical details on github: https://github.com/etzl/among-stars\nMove up and down in this window using <ARROW_UP> and <ARROW_DOWN> respectively, you can quit this help window anytime with <Q>. The goal of the game is to destroy as many ships as possible before you die. The number of destroyed ships are shown on the right corner of the screen as *KP* and the player's current health is shown as *HP*. As you might know, you can move the player with <ARROW_KEYS> or <W>, <A>, <S>, <D> and shoot with **either** <ENTER> or <SPACE>. You can pause the game anytime when playing, the *Resume* item will be enabled so you can continue playing."};
constexpr short M_ESC {27};

// windows
constexpr short M_POINTSWIN_NLINES {4};
constexpr short M_POINTSWIN_NCOLS {0};
constexpr short M_POINTSWIN_BEGY {0};
#define M_POINTSWIN_BEGX COLS-10

constexpr short M_MSGWIN_NLINES {0};
constexpr short M_MSGWIN_NCOLS {0};
#define M_MSGWIN_BEGY LINES-4
#define M_MSGWIN_BEGX COLS-10

constexpr short M_MENUWIN_NLINES {12};
constexpr short M_MENUWIN_NCOLS {50};
#define M_MENUWIN_BEGY (LINES-M_MENUWIN_NLINES)/2
#define M_MENUWIN_BEGX (COLS-M_MENUWIN_NCOLS)/2

// these positions are relative to the menu window
constexpr short M_MENUSUBWIN_NLINES {M_MENUWIN_NLINES - 2};
constexpr short M_MENUSUBWIN_NCOLS {20};
constexpr short M_MENUSUBWIN_BEGY {2};
constexpr short M_MENUSUBWIN_BEGX {4};

constexpr short M_DESCWIN_NLINES {M_MENUWIN_NLINES - 2};
constexpr short M_DESCWIN_NCOLS {30};
constexpr short M_DESCWIN_BEGY {1};
constexpr short M_DESCWIN_BEGX {M_MENUWIN_NCOLS - M_DESCWIN_NCOLS - 1};

constexpr short M_MENUFORMAT_ROWS {M_MENUSUBWIN_NLINES - 2};
constexpr short M_MENUFORMAT_COLS {1};

// colors
constexpr short M_GREENCOLOR_PAIR {1};
constexpr short M_REDCOLOR_PAIR {2};
constexpr short M_WHITECOLOR_PAIR {3};
constexpr short M_BLUECOLOR_PAIR {4};

#define M_MENU_CURSOR "* "

constexpr int M_MENUOPTS_OFF {O_SHOWDESC}; /* options to turn off from menu */

const std::vector<std::vector<const char*>> M_ITEMS_TXT {
    {"Resume", "Continue where you left"},
    {"New Game", "Start a fresh game"},
    {"Help", "How to play"},
    {"Exit", nullptr} // no description
};

const std::vector<std::vector<const char*>> M_CMDLINE_ARGS {
    {"--no-menu", "Go directly to the game (without showing menu)"},
    {"--no-damage", " Die hard - sets the player health to maximum possible value"},
    {"--help", "Show this message and quit"}
};

// startup options
struct Start_Opts {
    bool nomenu = false;
    bool nodamage = false;
    bool help = false;
};

/* ====================GLOBAL VARIABLES==================== */
PANEL *m_stdpnl, *m_msgpnl;
WINDOW *m_menuwin, *m_msgwin, *m_descwin;
MENU* m_mainmenu;
Stats m_stats;
std::vector<ITEM*> m_items;
std::mutex m_access_curses;
std::atomic_bool m_thread_ran {false};
bool m_menu {false};
bool m_quit {false};


void init(const Start_Opts&);    /* initialize objects */
void input();   /* get user input */
void update();  /* update (calculate) new positions */
void draw();    /* update screen */
void showmessage(std::string_view);    /* show a message for specified time */
void free_mem();  /* free memory */
void showmenu();
void text_buffer(WINDOW*, const char*); /* print characters in a buffer on a window */
void gameover(); /* Game over function */


// user can turn options on/off with command line arguments
Start_Opts checkargs(int count, char* argv[])
{
    Start_Opts op;
    for (int i=0; i!=count; ++i) {
        if (std::strcmp(argv[i], M_CMDLINE_ARGS[0][0]) == 0) {
            op.nomenu = true;
        }
        else if (std::strcmp(argv[i], M_CMDLINE_ARGS[1][0]) == 0) {
            op.nodamage = true;
        }
        else if (std::strcmp(argv[i], M_CMDLINE_ARGS[2][0]) == 0) {
            for (const auto& _arg: M_CMDLINE_ARGS) {
                std::cout << _arg[0] << '\t' << _arg[1] << '\n';
            }
            op.help = true;
        }
    }
    return op;
}

int main(int argc, char* argv[])
{
    Start_Opts opts; // options
    if (0 < argc)
        opts = checkargs(argc, argv);

    if (!opts.help) {
        init(opts);
        // main loop
        while (!m_quit) {
            auto tp1 = std::chrono::steady_clock::now();

            input();
            update();   // logical update
            draw(); // update frame

            auto tp2 = std::chrono::steady_clock::now();
            Game_manager::deltatime = std::chrono::duration<float>(tp2-tp1).count();
            if (m_menu) { // dont include the time we've spent in the menu
                m_menu = false;
                Game_manager::deltatime = 0;
            }
        }

        free_mem();
        endwin();
    }
    return 0;
}

void init(const Start_Opts& opts)
{
    // curses initializations
    initscr();
    cbreak();
    nl();
    noecho();
    curs_set(0);
    timeout(_timeoutms.count());
    keypad(stdscr, TRUE);

    // colors
    init_pair(M_GREENCOLOR_PAIR, COLOR_GREEN, 0);
    init_pair(M_REDCOLOR_PAIR, COLOR_RED, 0);
    init_pair(M_WHITECOLOR_PAIR, COLOR_WHITE, 0);
    init_pair(M_BLUECOLOR_PAIR, COLOR_BLUE, 0);

    // windows
    m_stats = newwin(M_POINTSWIN_NLINES, M_POINTSWIN_NCOLS, M_POINTSWIN_BEGY, M_POINTSWIN_BEGX);
    m_msgwin = newwin(M_MSGWIN_NLINES, M_MSGWIN_NCOLS, M_MSGWIN_BEGY, M_MSGWIN_BEGX);
    m_menuwin = newwin(M_MENUWIN_NLINES, M_MENUWIN_NCOLS, M_MENUWIN_BEGY, M_MENUWIN_BEGX);

    m_descwin = derwin(m_menuwin, M_DESCWIN_NLINES, M_DESCWIN_NCOLS, M_DESCWIN_BEGY, M_DESCWIN_BEGX);

    // menu items
    m_items.reserve(M_ITEMS_TXT.size() + 1); // including nullptr
    for (const auto& _items: M_ITEMS_TXT) {
        m_items.push_back(new_item(_items[0], _items[1])); // item_name, description
    }
    m_items.push_back(nullptr); // last item has to be null (see menu documentation)

    item_opts_off(m_items[0], O_SELECTABLE);
    keypad(m_menuwin, TRUE);

    m_mainmenu = new_menu(m_items.data());

    // menu customizations
    set_menu_win(m_mainmenu, m_menuwin);
    set_menu_sub(m_mainmenu, derwin(m_menuwin, M_MENUSUBWIN_NLINES, M_MENUSUBWIN_NCOLS,
    M_MENUSUBWIN_BEGY, M_MENUSUBWIN_BEGX));
    set_menu_format(m_mainmenu, M_MENUFORMAT_ROWS, M_MENUFORMAT_COLS);
    menu_opts_off(m_mainmenu, M_MENUOPTS_OFF);
    set_menu_mark(m_mainmenu, M_MENU_CURSOR);

    m_stdpnl = new_panel(stdscr);
    m_msgpnl = new_panel(m_msgwin);

    update_panels();
    doupdate();

    Game_manager::player = Player{static_cast<float>(_Player_initial_y),
     static_cast<float>(_Player_initial_x)};

#ifndef _DEBUG
    if (!opts.nomenu)
        showmenu();
    if (opts.nodamage)
        Game_manager::player.health_cheat() = std::numeric_limits<int>::max();
#endif

    item_opts_on(m_items[0], O_SELECTABLE);
}

void free_mem()
{
    unpost_menu(m_mainmenu);
    free_menu(m_mainmenu);
    for (auto& it: m_items)
        free_item(it);
    // delete associate panels
    del_panel(m_stdpnl);
    del_panel(m_msgpnl);
    // delete windows
    delwin(m_menuwin);
    delwin(m_msgwin);
    delwin(m_descwin);
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
            m_quit = true;
            break;
        case M_ESC:
            showmenu();
            break;
        default:
            showmessage(std::string_view{"Wrong input!"});
            break;
    }
}

void draw()
{
    std::scoped_lock lock {m_access_curses};

    erase();

    auto drw = [](const Drawable_obj& obj) { obj.draw(stdscr); };

    drw(Game_manager::player);
    std::for_each(Game_manager::enemies.begin(), Game_manager::enemies.end(),
        drw);
    std::for_each(Game_manager::bullets.begin(), Game_manager::bullets.end(),
        drw);

    // show player stats
    m_stats.draw();

    update_panels();
    doupdate();
}

// design (prepare) the window with title
void design_w(WINDOW* win, const char* title)
{
    int maxx = getmaxx(win);
    werase(win);
    box(win, 0, 0);
    mvwaddstr(win, 0 , (maxx-strlen(title))/2, title);
    wrefresh(win);
}

/* This function used only by showmessage() */
void task_showmessage(const std::string_view msg)
{
    std::unique_lock lck {m_access_curses};

    show_panel(m_msgpnl);
    text_buffer(m_msgwin, msg.data());

    update_panels();
    doupdate();

    lck.unlock();
    std::this_thread::sleep_for(_Showmessage_timer);
    lck.lock();

    hide_panel(m_msgpnl);
    update_panels();
    doupdate();

    m_thread_ran = false;
}
void showmessage(std::string_view msg)
{
    if (m_thread_ran)
        return;

    m_thread_ran = true;
    std::thread task {task_showmessage, msg};
    task.detach();
}

void update()
{
    Game_manager::generate_enemies();
    Game_manager::move_enemies();
    Game_manager::shoot();
    Game_manager::update();

    if (Game_manager::player.isdead) {
        m_stats.draw();
        update_panels();
        doupdate();
        gameover();
    }
}

void showmenu_desc()
{
    design_w(m_descwin, "Description");
    text_buffer(m_descwin, item_description(current_item(m_mainmenu)));
}

void show_help()
{
    unpost_menu(m_mainmenu);
    // change background color of the menu here...
    text_buffer(m_menuwin, Help_description.data());
    werase(m_menuwin);
    wbkgd(m_menuwin, 0);
    // set default color here...
    box(m_menuwin, 0, 0);
    mvwaddstr(m_menuwin, 0, 2, "Menu!");
    post_menu(m_mainmenu);
    showmenu_desc();
    wrefresh(m_menuwin);
}

void gameover()
{
    wclear(m_menuwin);
    // change background color here
    int maxx, maxy;
    getmaxyx(m_menuwin, maxy, maxx);

    box(m_menuwin, 0, 0);
    char mesg[] = "GAME OVER!";
    mvwaddstr(m_menuwin, maxy/2, (maxx-std::strlen(mesg))/2, mesg);
    refresh();
    wrefresh(m_menuwin);
    std::this_thread::sleep_for(_Gameover_timer);

    werase(m_menuwin);
    wbkgd(m_menuwin, 0);
    // set default background color here
    item_opts_off(m_items[0], O_SELECTABLE);
    showmenu();
}

void showmenu()
{
    m_menu = true;
    // update background!
    erase();
    refresh();

    box(m_menuwin, 0, 0);
    mvwaddstr(m_menuwin, 0, 2, "Menu!");
    post_menu(m_mainmenu);
    showmenu_desc();
    wrefresh(m_menuwin);

    bool menu_loop = true;
    while (menu_loop) {
        int ch = wgetch(m_menuwin);
        switch (ch) {
            case KEY_DOWN:
            case 'j':
            case 's':
                menu_driver(m_mainmenu, REQ_DOWN_ITEM);
                showmenu_desc();
                break;
            case KEY_UP:
            case 'k':
            case 'w':
                menu_driver(m_mainmenu, REQ_UP_ITEM);
                showmenu_desc();
                break;
            case M_ESC:
                menu_loop = false;
                break;
            case '\n':
                ITEM* cur_item = current_item(m_mainmenu);
                if (item_opts(cur_item) & O_SELECTABLE) {
                    std::string iname {item_name(cur_item)};
                    if (iname == "Exit") {
                        m_quit = true;
                        menu_loop = false;
                    }
                    else if (iname == "New Game") {
                        if (item_opts(cur_item) & O_SELECTABLE) {
                            Game_manager::restart();
                            menu_loop = false;
                        }
                    }
                    else if (iname == "Resume") {
                        if (item_opts(cur_item) & O_SELECTABLE) {
                            menu_loop = false;
                        }
                    }
                    else if (iname == "Help") {
                        show_help();
                    }
                }
                break;
        }
        wrefresh(m_menuwin);
    }

    unpost_menu(m_mainmenu);
    wrefresh(m_menuwin);
}

void text_buffer(WINDOW* place, const char* msg)
{
    using namespace std;

    if (msg == nullptr)
        return;
    string_view desc {msg};

    // returns true if the character is not an attribute character
    auto not_attr = [](const char x) {
        constexpr string_view attrib = "*\n\b";
        return attrib.find(x) == string_view::npos;
    };

    static const regex pattern {R"((\S+\s*))"};
    int bufl = 0, bufc = 0, winattrs = 0, prev = 0;

    // do not use these two variables; use 'maxc' and 'maxl' instead
    int maxx, maxy;
    getmaxyx(place, maxy, maxx);
    const int maxc = maxx - 2, maxl = maxy - 2; // sizes which we can actually use
    int m_line = maxl;

    design_w(place, "Description");

    vector<chtype> buffer; buffer.reserve(m_line*maxc);

    /* write white-space until the end of current line
     * column: position to start in the line; passing variable 'bufc' causes to write
     * white-space to the remaining characters in the buffer
     */
    auto newline_buf = [&](int column) {
        bufc = 0; bufl++;
        int size = maxc - column;
        while (size-- > 0) {
            buffer.push_back(32);
        }
    };

    bool bflag = false;
    int lsize = 0;

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
            if (*ch == '<') {
                // winattrs |= COLOR_PAIR(5);
            }
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

                lsize -= wsize;

                // find next \b
                for (++iter; iter!=cregex_iterator{}; iter++) {
                    string_view tmp = iter->str(1);
                    word += tmp;
                    if (tmp.find('\b') != string::npos) {
                        bflag = true;
                        break;
                    }
                }

                if (!bflag) // probably not what we wanted!
                    throw "another \\b character not found";

                // word iterators are undefined
                ch = word.begin();
                // count new length
                wsize = count_if(ch+1, word.end(), not_attr);
                lsize += wsize;

                if (wsize >= maxc)
                    throw "characters between \\b can't be larger than max column size of the window";

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

            // finally write to the buffer
            buffer.push_back(*ch | winattrs);
            // count newly added character
            bufc++;
            prev = *ch;

            if (*ch == '>') {
                // winattrs ^= COLOR_PAIR(5);
            }
        }
    }

    // print buffer
    bufl=0; // mark the begging of the buffer - we will start printing from here
    int winline = 0; // track current line in the window (starting from 1)
    const double maxbufl = std::ceil(static_cast<double>(buffer.size()) / maxc);
    bool interactive = false;
    const auto lch_size = buffer.size() % maxc;
    // max value of bufl; before exceeding the buffer
    const auto mbufl = maxbufl - maxl;

    for (int line = bufl; ; ++line) {
        if (line == maxbufl && !interactive)
            break;
        wmove(place, ++winline, 1);

        // window lines exceeded - go to interactive mode
        if (winline > maxl) {
            interactive = true;
            get_input:
                int ch = wgetch(place);
                switch (ch) {
                    case 'k':
                    case KEY_UP:
                        if (bufl > 0) {
                            line = --bufl;
                            break;
                        }
                        goto get_input;
                    case 'j':
                    case KEY_DOWN:
                        if (bufl < mbufl) {
                            line = ++bufl;
                            break;
                        }
                        goto get_input;
                    case 'q':
                        goto break_interactive;
                    default:
                        goto get_input;
                }

            design_w(place, "Description");
            winline = 1;
            wmove(place, winline, 1);
        }

        for (bufc = 0; bufc < maxc; bufc++) {
            if (line == maxbufl - 1) { // this is last line in the buffer
                if (bufc == lch_size && lch_size != 0)
                    break;
            }
            waddch(place, buffer.at(line*maxc + bufc));
        }
    }

    break_interactive:
        wrefresh(place);
}
