#include <curses.h>
#include <cmath>



// diagonally movements are going to be 45 degree
const float _sin = std::sin(45);
const float _cos = std::cos(45);

enum class Dir {
    up = 1, down = -1, // just in case Dir::up, Dir::down became tedious
    right = 1, left = -1,
    none
};


class Drawable_obj {
public:
    Drawable_obj(const int y, const int x, const int speed) :x{x}, y{y}, speed{speed}
    {}

    virtual void draw() const = 0;

    virtual int getx() const { return x; }
    virtual int gety() const { return y; }
    virtual int getspeed() const { return speed; }

    virtual void setx(int newx) { x = newx; }
    virtual void sety(int newy) { y = newy; }

    virtual void move(const Dir dirx, const Dir diry)
    {
        if ((int)dirx == 0)
            y += speed * (int)diry;
        else if ((int)diry == 0)
            x += speed * (int)dirx;
        else {
            x += _cos * (speed * (int)dirx);
            y += _sin * (speed * (int)diry);
        }

        /* curses accept integer so this might discard any changes we've made */
        x = std::round(x);
        y = std::round(y);
    }
protected:
    int x,y,speed;
};
