#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H


#include <vector>
#include <chrono>

#include "objects.h"


constexpr int _Max_enemy_per_row = 4;   /* Maximum allowed enemies while generating a new line of enemies */
constexpr int _Min_enemy_per_row = 1;   /* Minimum number of allowed enemies while generating */
constexpr int _Enemy_maximum_generate = 4; /* If enemies are less than this number we are allowed to regenerate more */
constexpr int _Points_perenemy = 1;

constexpr float _High_power_speed_increase = 6;
constexpr float _High_distance_from_begin = 1;

constexpr float _Allowed_distance_from_end = 8; /* We can only reach this distance from bottom (when in high speed mode) */
constexpr float _Allowed_distance_from_corner = 6;   /* The distance from right or left of the screen */

constexpr std::chrono::seconds _Showmessage_timer(1);
constexpr std::chrono::seconds _Shoot_interval(3);
constexpr std::chrono::milliseconds _timeoutms(50);
constexpr std::chrono::seconds _Gameover_timer(3);

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