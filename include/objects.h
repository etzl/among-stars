#ifndef OBJECTS_H
#define OBJECTS_H

#include "drawable.h"
#include "bullet.h"


constexpr float _enemyspeed = 0.5;
constexpr int _playerspeed = 1;
constexpr int _player_maxhealth = 10;
constexpr int _enemy_maxhealth = 1;


class Interactable :public Drawable_obj {
public:
    Interactable(const int h, const int y, const int x, const float speed):
    Drawable_obj(y, x, speed), health{h}
    {}

    void hit(int damage)
    {
        health -= damage;
    }
    int gethealth() { return health; }
    /* check if x is in the range that our shape occupied */
    bool inrange(int x) const =0;

protected:
    int health;
};

class Enemy :public Interactable {
public:
    Enemy(const int y, const int x) :Interactable(_enemy_maxhealth, y, x,
    _enemyspeed) {}

    Bullet shoot()
    {
        return Bullet(y+1, x, Dir::down);
    }
    void draw() const override
    {
        mvaddstr(y, x-2, ">||<");
    }
    bool inrange(int chkx) const override
    {
        return (chkx >= x-2) && (chkx <= x+1);
    }
};

class Player : public Interactable {
public:
    Player(const int y, const int x) :Interactable(_player_maxhealth, y, x,
    _playerspeed) {}

    Bullet shoot()
    {
        return {y-1, x, Dir::up};
    }
    void draw() const override
    {
        mvaddstr(y, x-1, "/_\\");
    }
    bool inrange(int chkx) const override
    {
        return (chkx >= x-1) && (chkx <= x+1);
    }
};

#endif // OBJECTS_H