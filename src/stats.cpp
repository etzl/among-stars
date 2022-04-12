#include "stats.hpp"
#include "objects.hpp"


constexpr short _Color_number {10}; // ncurses default colors are 1 to 8; 10 might be a good value for our own
constexpr short _Color_pair {8}; // refer to main.cpp

void change_color()
{
    // a percentage of current health over max health
    const float health_degree {static_cast<float>(Game_manager::player.gethealth())
        / static_cast<float>(_Player_maxhealth)};
    const float health_opp_degree {1.0f - health_degree};

    // increase red color as our health drops
    init_color(_Color_number, static_cast<short>(health_opp_degree*1000),
        static_cast<short>(health_degree*1000), 0);
}

void Stats::draw()
{
    werase(win);
    box(win, 0, 0);

    change_color();
    _health.assign("HP: " + std::to_string(Game_manager::player.gethealth()));
    wattron(win, COLOR_PAIR(_Color_pair));
    mvwaddstr(win, _Health_begy, _Health_begx, _health.c_str());
    wattroff(win, COLOR_PAIR(_Color_pair));

    _points.assign("KP: " + std::to_string(Game_manager::player_points));
    wattron(win, COLOR_PAIR(5));
    mvwaddstr(win, _Points_begy, _Points_begx, _points.c_str());
    wattroff(win, COLOR_PAIR(5));
}
