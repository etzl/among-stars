#ifndef BULLET_H
#define BULLET_H


#include "drawable.h"


constexpr float _bulletspeed = 20;



class Bullet :public Drawable_obj {
public:
    Bullet(float y, float x, Dir d) :Drawable_obj{y, x, _bulletspeed},
    direction{d}
    {}
    bool update();
    size_t size() const override { return 1; }
    void draw(WINDOW* plac) const override  // should later be changed to perform on stdscr
    {
        mvwaddch(plac, y, x, ACS_BULLET);
    }
private:
    Dir direction = Dir::none;
};

#endif // BULLET_H