// #define _DEBUG

#include <memory>
#include <cstring>

#include "gamemanager.h"
#include "curse_windows.h"



std::unique_ptr<Curse_windows> __winhandle;


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

    __winhandle = std::make_unique<Curse_windows>(new Curse_windows(runwithoutmenu));

    while (true) {
        auto tp1 = std::chrono::steady_clock::now();

        __winhandle->input();
        __winhandle->update();
        __winhandle->draw();

        auto tp2 = std::chrono::steady_clock::now();
        Game_manager::deltatime = std::chrono::duration<float>(tp2-tp1).count();
        if (__winhandle->menu) { // dont include the time we've spent in the menu, also give player 1 frame to think about
            __winhandle->menu = false;
            Game_manager::deltatime = 0;
        }
    }
}
