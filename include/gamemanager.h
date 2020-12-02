#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H


#include <vector>
#include "objects.h"





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