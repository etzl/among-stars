#ifndef STATS_HPP
#define STATS_HPP


#include <string>
#include "drawable.hpp"
#include "gamemanager.hpp"


constexpr short _Health_begy {1};
constexpr short _Health_begx {1};
constexpr short _Points_begy {2};
constexpr short _Points_begx {1};

class Stats {
    public:
        Stats() =default;

        Stats(WINDOW* _pointswin)
        {
            win = _pointswin;
            panel = new_panel(win);
        }

        Stats(Stats&& oth) :win{oth.win}, panel{oth.panel}
        {}

        Stats(const Stats&) =delete;

        Stats& operator=(WINDOW* _pointswin)
        {
            del_panel(panel);
            delwin(win);

            win = _pointswin;
            panel = new_panel(win);

            return *this;
        }
        Stats& operator=(Stats&& oth)
        {
            del_panel(panel);
            delwin(win);

            win = oth.win;
            panel = oth.panel;

            return *this;
        }

        Stats& operator=(const Stats&) =delete;

        void draw()
        {
            werase(win);
            box(win, 0, 0);

            _health.assign("HP: " + std::to_string(Game_manager::player.gethealth()));
            mvwaddstr(win, _Health_begy, _Health_begx, _health.c_str());

            _points.assign("KP: " + std::to_string(Game_manager::player_points));
            mvwaddstr(win, _Points_begy, _Points_begx, _points.c_str());
        }

        ~Stats()
        {
            del_panel(panel);
            delwin(win);
        }
    private:
        std::string _health;
        std::string _points;
        WINDOW* win;
        PANEL* panel;
};

#endif  // STATS_HPP_