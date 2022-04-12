#ifndef STATS_HPP_
#define STATS_HPP_


#include <string>
#include <panel.h>

#include "drawable.hpp"
#include "gamemanager.hpp"


constexpr short _Health_begy {1};
constexpr short _Health_begx {1};
constexpr short _Points_begy {2};
constexpr short _Points_begx {1};

class Stats {
    public:
        Stats() =default;
        Stats(const Stats&) =delete;
        Stats& operator=(const Stats&) =delete;

        Stats(WINDOW* _pointswin)
        {
            win = _pointswin;
            panel = new_panel(win);
        }

        Stats(Stats&& oth) :win{oth.win}, panel{oth.panel}
        {}


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

        /** Get underline window associated with stats object
         * \return A pointer to the underline window
         */
        WINDOW *getwin()
        {
            return win;
        }

        /** Draw player's stats on the screen
         * \return void
         */
        void draw();

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