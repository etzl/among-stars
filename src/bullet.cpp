#include "bullet.h"
#include "gamemanager.h"
#include "objects.h"


constexpr int _bulletdamage = 1;


/* returning true deletes the bullet from game manager list */
bool Bullet::update()
{
    move(Dir::none, direction);
    if (direction == Dir::up)
        for (auto& enemy: Game_manager::enemies)
            if (enemy.gety() == y || enemy.gety() == (y-1))
                if (enemy.inrange(x)) {
                    enemy.hit(_bulletdamage);
                    return true;
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
