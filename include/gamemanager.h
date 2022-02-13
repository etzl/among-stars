#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H


#include <vector>
#include <chrono>

#include "objects.h"


constexpr int _max_enemy_per_row = 4;
constexpr int _min_enemy_per_row = 1;
constexpr int _Enemy_maximum_generate = 4; /* If enemies are less than this number we are allowed to regenerate more */
constexpr int _Points_perenemy = 1;

constexpr float _Allowed_distance_from_end = 8;
constexpr float _High_power_speed_increase = 6;
constexpr float _High_distance_from_begin = 3;

constexpr float _Relative_corner = 4;   /* The distance from right or left of the screen */

constexpr std::chrono::seconds _Showmessage_timer(1);
constexpr std::chrono::seconds _Shoot_interval(3);
constexpr std::chrono::milliseconds _timeoutms(50);
constexpr std::chrono::seconds GAMEOVER_TIMER(3);

#define _Player_initial_x COLS/2
#define _Player_initial_y (LINES/2)+4




class Game_manager {
public:
    static int player_points;
    static std::vector<Enemy> enemies;
    static std::vector<Bullet> bullets;
    static Player player;
    static float deltatime;


    static void generate_enemies();
    static void move_enemies();
    static void update(); /* update frame */
    static void shoot(); /* enemy shoot logic */
    static void restart(); /* restart variables to their initial state */
};


#endif  // GAME_MANAGER_H