#ifndef OBJECTS_HP_
#define OBJECTS_HP_

#include "drawable.hpp"


constexpr float _Enemy_speed {5};
constexpr float _Player_speed {20};
constexpr int _Player_maxhealth {10};
constexpr int _Enemy_maxhealth {1};


class Bullet;

enum class Enemy_states {
    idle, high, up_left, up_right, down_left, down_right
};

class Interactable :public Drawable_obj {
public:
    Interactable(const int h, const float y, const float x, const float speed):
    Drawable_obj(y, x, speed), health{h}
    {}

    void hit(int damage)
    {
        health -= damage;
    }
    int gethealth()
    {
        return health;
    }

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
    Player(const float y, const float x) :Interactable(_Player_maxhealth, y, x,
    _Player_speed) {}

    Bullet shoot();
    void draw(WINDOW* plac) const override
    {
        mvwaddstr(plac, y, x-1, R"(/_\)");
    }
    size_t size() const override
    {
        return 3;
    }
    bool inrange(float chkx) const override
    {
        return (chkx >= x-1) && (chkx <= x+1);
    }
    int& health_cheat()
    {
        return health;
    }

    bool isdead = false;
};


class Enemy :public Interactable {
public:
    Enemy(const float y, const float x) :Interactable(_Enemy_maxhealth, y, x,
    _Enemy_speed) {}

    Bullet shoot();

    void draw(WINDOW* plac) const override
    {
        mvwaddstr(plac, y, x-2, R"(>||<)");
    }
    size_t size() const override
    {
        return 4;
    }
    bool inrange(float chkx) const override
    {
        return ((x-2) <= chkx) && (chkx <= x+1);
    }
    Enemy_states& mode()
    {
        return state;
    }

private:
    Enemy_states state = Enemy_states::idle;
};

// struct Window_prop {
//     constexpr Window_prop(int y, int x, int l, int c) :y{y}, x{x}, lines{l},
//     cols{c} {}
//     int y, x, lines, cols;
// };

#endif // OBJECTS_HPP_