#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H


#include <vector>
#include "objects.h"


constexpr int _bulletdamage = 1;


class Game_manager {
public:
    void generate_enemies();
    void move_enemies();
    void collide();
private:
    static int player_points;
    static std::vector<Enemy> enemies;
    static std::vector<Bullet> bullets;
    static Player player;
};


#endif  // GAME_MANAGER_H