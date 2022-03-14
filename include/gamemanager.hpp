#ifndef GAME_MANAGER_HP_
#define GAME_MANAGER_HP_


#include <vector>
#include <chrono>

#include "objects.hpp"


constexpr int _Max_enemy_per_row {4};   /* Maximum allowed enemies while generating a new line of enemies */
constexpr int _Min_enemy_per_row {1};   /* Minimum number of allowed enemies while generating */
constexpr int _Enemy_maximum_generate {4}; /* If enemies are less than this number we are allowed to regenerate more */
constexpr int _Points_perenemy {1};

constexpr float _High_power_speed_increase {6};
constexpr float _High_distance_from_begin {1};

constexpr float _Allowed_distance_from_end {8}; /* We can only reach this distance from bottom (when in high speed mode) */
constexpr float _Allowed_distance_from_corner {6};   /* The distance from right or left of the screen */

constexpr std::chrono::seconds _Showmessage_timer {1};
constexpr std::chrono::seconds _Shoot_interval {3};
constexpr std::chrono::milliseconds _timeoutms {50};
constexpr std::chrono::seconds _Gameover_timer {3};

#define _Player_initial_x COLS/2
#define _Player_initial_y (LINES/2)+4
#define _Enemy_first_allowed_column 2
#define _Enemy_last_allowed_column COLS-3


class Game_manager {
    public:
        /** Generates new enemies based on the algorithm
         * \return void
         */
        static void generate_enemies();

        /** Move enemies based on the algorithm
         * \return void
         */
        static void move_enemies();

        /** Update frame - logically
         * \return void
         */
        static void update();

        /** Shoot a bullet off each enemy objects on a time interval
         * \return void
         */
        static void shoot();

        /** Restart the game
         * \return void
         */
        static void restart();

        static int player_points;
        static std::vector<Enemy> enemies;
        static std::vector<Bullet> bullets;
        static Player player;
        static float deltatime;
};


#endif  // GAME_MANAGER_HP_