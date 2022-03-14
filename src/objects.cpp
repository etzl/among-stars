#include "objects.hpp"
#include "bullet.hpp"


Bullet Player::shoot()
{
    return {y, x, Dir::up};
}

Bullet Enemy::shoot()
{
    return {y, x, Dir::down};
}