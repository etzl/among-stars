#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H


#include <vector>
#include <chrono>

#include "objects.h"


constexpr int _max_enemy_per_row = 4;
constexpr int _min_enemy_per_row = 1;
constexpr int _Magic_enemy = 4; /* A magic number so generation of enemies won't happen very often */
constexpr int _Points_perenemy = 1;

constexpr float _Allowed_distance_from_end = 8;
constexpr float _High_power_speed_increase = 0.3F;
constexpr float _High_distance_from_begin = 3;

constexpr float _Relative_corner = 4;   /* The distance from right or left of the screen */

constexpr int _timeout = 50;
constexpr std::chrono::seconds _Showmessage_timer(1);
constexpr float _Shoot_interval = 3000; // milliseconds




class Game_manager {
public:
    static int player_points;
    static std::vector<Enemy> enemies;
    static std::vector<Bullet> bullets;
    static Player player;


    static void generate_enemies();
    static void move_enemies();
    /* Update frame */
    static void update();
    /* Enemies shoot logic */
    static void shoot();
};


#endif  // GAME_MANAGER_H