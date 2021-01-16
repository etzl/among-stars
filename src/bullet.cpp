#include "bullet.h"
#include "gamemanager.h"
#include "objects.h"


constexpr int _bulletdamage = 1;


/* returning true deletes the bullet from game manager list */
bool Bullet::update()
{
    move(Dir::none, direction);
    if (direction == Dir::up) {
        // biggest distance means the enemy we passed first (to attack)
        float lastbigdistance = 0;
        int index;
        bool hit = false;

        for (int i=0; i<Game_manager::enemies.size(); ++i)
            if (Game_manager::enemies[i].inrange(x)) {
                float distance = Game_manager::enemies[i].gety() - y;
                // we hit or already passed the enemy
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
        if (Game_manager::player.gety() == y)
            if (Game_manager::player.inrange(x)) {
                Game_manager::player.hit(_bulletdamage);
                return true;
            }

    if (y <= 0 || y >= LINES)
        return true;

    return 0;
}
