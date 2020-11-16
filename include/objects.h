#ifndef OBJECTS_H
#define OBJECTS_H

#include "drawable.h"
#include "bullet.h"


constexpr int _enemyspeed = 1;
constexpr int _playerspeed = 1;


class Enemy :public Drawable_obj {
public:
    Enemy(const int y, const int x) :Drawable_obj(y,x,_enemyspeed)
    {}

    Bullet shoot()
    {
        return Bullet(y+1, x, Dir::down);
    }
    void draw() const override
    {
        mvaddstr(y, x-2, ">||<");
    }
};

class Player : public Drawable_obj {
public:
    Player(const int y, const int x) :Drawable_obj(y,x,_playerspeed)
    {}

    Bullet shoot()
    {
        return {y-1, x, Dir::up};
    }
    void draw() const override
    {
        mvaddstr(y, x-1, "/_\\");
    }
};

#endif // OBJECTS_H