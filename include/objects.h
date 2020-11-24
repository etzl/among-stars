#ifndef OBJECTS_H
#define OBJECTS_H

#include "drawable.h"
#include "bullet.h"


constexpr float _enemyspeed = .5;
constexpr int _playerspeed = 1;
constexpr int _player_maxhealth = 10;
constexpr int _enemy_maxhealth = 1;


class Interactable :public Drawable_obj {
public:
    Interactable(const int h, const int y, const int x, const float speed):
    Drawable_obj(y, x, speed), health{h}
    {}

    /* Compare only with positions */
    bool operator==(const Interactable& com) const
    {
        return (x == com.x) && (y == com.y);
    }
    bool operator!=(const Interactable& com) const
    {
        return (x != com.x) || (y != com.y);
    }
    bool operator<(const Interactable& com) const
    {
        return (x < com.x) && (y < com.y);
    }
    bool operator>(const Interactable& com) const
    { return (x > com.x) && (y > com.y); }

    bool operator<=(const Interactable& com) const
    { return (*this < com) || (*this == com); }

    bool operator>=(const Interactable& com) const
    { return (*this > com) || (*this == com); }



    /* dead objects will handle by game manger */
    int getdamage(int damage)
    {
        health -= damage;
        if (health <= 0) // we are dead XD
            return 1;
        return 0;
    }

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
};

#endif // OBJECTS_H