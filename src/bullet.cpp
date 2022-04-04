#include "bullet.hpp"
#include "gamemanager.hpp"
#include "objects.hpp"


constexpr int _bulletdamage = 1;


bool Bullet::update()
{
    move(direction);
    if (direction == Dir::up) {
        float lastbigdistance = 0;
        size_t index;
        bool hit = false;

        for (size_t i=0; i<Game_manager::enemies.size(); ++i)
            if (Game_manager::enemies[i].inrange(x)) {
                float distance = Game_manager::enemies[i].gety() - y;
                // check if we hit or already passed the enemy
                // implicitely check for distance >= 0
                if (lastbigdistance <= distance) {
                    lastbigdistance = distance;
                    index = i;
                    hit = true;
                }
            }

        if (hit) {
            Game_manager::enemies[index].hit(_bulletdamage);
            return 1;
        }
    }
    else
        if (Game_manager::player.inrange(x)) {
            float distance = y - Game_manager::player.gety();
            if (0 <= distance) {
                Game_manager::player.hit(_bulletdamage);
                return 1;
            }
        }

    // destroy out of screen bullets
    if (y <= 0 || y >= static_cast<float>(LINES))
        return true;

    return 0;
}
