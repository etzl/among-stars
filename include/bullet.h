
#ifndef BULLET_H
#define BULLET_H

#include <objects.h>

constexpr int _bulletspeed = 1;


class Bullet : public Drawable_obj {
public:
    Bullet(int y, int x, Dir d) :Drawable_obj(y,x,_bulletspeed), direction{d}
    {}

    /* return 1 when passing the screen */
    int update()
    {
        move(Dir::none, direction);
        if (y < 0 || y > maxy)
            return 1;
        return 0;
    }

    void draw() const override
    {
        mvaddch(y, x, ACS_BULLET);
    }
private:
    Dir direction;
    int maxy = getmaxy(stdscr);
};


#endif // BULLET_H