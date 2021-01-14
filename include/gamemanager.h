#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H


#include <vector>

#include "objects.h"


constexpr int _max_enemy_per_row = 3;
constexpr int _min_enemy_per_row = 0;
// A magic number so generation of enemies won't happen very often
constexpr int _Magic_enemy = 5;
constexpr int _Points_perenemy = 1;



class Game_manager {
public:
    static int player_points;
    static std::vector<Enemy> enemies;
    static std::vector<Bullet> bullets;
    static Player player;


    static void generate_enemies();
    static void move_enemies();
    static void update();
};


#endif  // GAME_MANAGER_H