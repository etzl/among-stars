#include "objects.h"
#include "bullet.h"


Bullet Player::shoot()
{
    return {y-1, x, Dir::up};
}

Bullet Enemy::shoot()
{
    return Bullet(y+1, x, Dir::down);
}