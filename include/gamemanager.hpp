#ifndef GAME_MANAGER_HP_
#define GAME_MANAGER_HP_


#include <vector>
#include <chrono>

#include "objects.hpp"

/* ==================MAGIC NUMBERS================ */
#define M_COLLISION_RANGE 2
#define M_MAX_COLLISION_RANGE_Y LINES-M_COLLISION_RANGE
#define M_MAX_COLLISION_RANGE_X COLS-M_COLLISION_RANGE /* distance from the right of the screen */

#define M_ENEMY_COLLISION_RANGE_X 4
#define M_ENEMY_MAX_COLLISION_RANGE_X COLS-M_ENEMY_COLLISION_RANGE_X

#define M_HIGH_GROUND M_COLLISION_RANGE /* this is the distance from the top (high ground) */
#define M_MAX_DISTANCE_END M_MAX_COLLISION_RANGE_Y-6 /* maximum possible value for enemies to get close to the player */

#define M_PLAYER_INIT_X COLS/2
#define M_PLAYER_INIT_Y M_MAX_COLLISION_RANGE_Y-3
#define M_ENEMY_FIRST_COL_GENERATE M_ENEMY_COLLISION_RANGE_X+2
#define M_ENEMY_LAST_COL_GENERATE M_ENEMY_MAX_COLLISION_RANGE_X-2

constexpr short _Max_enemy_per_row {4};   /* Maximum allowed enemies while generating a new line of enemies */
constexpr short _Min_enemy_per_row {1};   /* Minimum number of allowed enemies while generating */
constexpr short _Enemy_maximum_generate {4}; /* If enemies are less than this number we are allowed to regenerate more */
constexpr short _Points_perenemy {1};

constexpr float _High_power_speed_increase {6};

constexpr std::chrono::seconds _Showmessage_timer {1};
constexpr std::chrono::seconds _Shoot_interval {3};
constexpr std::chrono::milliseconds _timeoutms {50};
constexpr std::chrono::seconds _Gameover_timer {3};


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