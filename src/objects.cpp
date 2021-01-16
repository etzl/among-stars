#include "objects.h"
#include "bullet.h"


Bullet Player::shoot()
{
    return {y, x, Dir::up};
}

Bullet Enemy::shoot()
{
    return Bullet(y, x, Dir::down);
}