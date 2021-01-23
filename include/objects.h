#ifndef OBJECTS_H
#define OBJECTS_H

#include "drawable.h"


constexpr float _enemyspeed = 0.5F;
constexpr float _playerspeed = 1;
constexpr int _player_maxhealth = 10;
constexpr int _enemy_maxhealth = 1;



class Bullet;


class Interactable :public Drawable_obj {
public:
    Interactable(const int h, const float y, const float x, const float speed):
    Drawable_obj(y, x, speed), health{h}
    {}

    void hit(int damage)
    {
        health -= damage;
    }
    int gethealth() { return health; }

    bool operator==(const Interactable& cmp)
    {
        return (cmp.health == health) && (Drawable_obj::operator==(cmp));
    }
    bool operator!=(const Interactable& cmp)
    {
        return !(*this == cmp);
    }

    /* check if x is in the range that our occupied shape */
    virtual bool inrange(float x) const =0;

protected:
    int health;
};


class Player : public Interactable {
public:
    Player(const float y, const float x) :Interactable(_player_maxhealth, y, x,
    _playerspeed) {}

    Bullet shoot();
    void draw(WINDOW* plac) const override
    {
        mvwaddstr(plac, y, x-1, R"(/_\)");
    }
    bool inrange(float chkx) const override
    {
        return (chkx >= x-1) && (chkx <= x+1);
    }
};


enum class Enemy_states {
    high, left, right
};
class Enemy :public Interactable {
public:
    Enemy(const float y, const float x) :Interactable(_enemy_maxhealth, y, x,
    _enemyspeed) {}

    Bullet shoot();
    void draw(WINDOW* plac) const override
    {
        mvwaddstr(plac, y, x-2, R"(>||<)");
    }
    bool inrange(float chkx) const override
    {
        return ((x-2) <= chkx) && (chkx <= x+1);
    }
    Enemy_states& mode() { return state; }

private:
    Enemy_states state;
};



// struct Window_prop {
//     constexpr Window_prop(int y, int x, int l, int c) :y{y}, x{x}, lines{l},
//     cols{c} {}
//     int y, x, lines, cols;
// };

#endif // OBJECTS_H