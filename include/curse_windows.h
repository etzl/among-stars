#ifndef WINDOWS_H
#define WINDOWS_H

#include <string_view>
#include <curses.h>
#include <panel.h>
#include <menu.h>
#include <vector>
#include <atomic>
#include <mutex>


// there should be only one instance of this type
class Curse_windows {
public:
    Curse_windows(bool);
    void draw();
    void showmenu();
    void showmessage(std::string_view);
    void showhelp();
    ~Curse_windows();

private:
    void task_showmessage(const std::string_view);
    PANEL *stdpnl, *pointspnl, *msgpnl;
    WINDOW *pointswin, *menuwin, *msgwin, *descwin;
    MENU *mainmenu;
    std::vector<ITEM *> m_items;
    std::mutex access_curses;
    std::atomic_bool thr_run = false;
};

#endif  // WINDOWS_H