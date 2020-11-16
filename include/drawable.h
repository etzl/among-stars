#include <curses.h>



enum class Dir {
    up = 1, down = -1, // just in case Dir::up, Dir::down became tedious
    none
};


class Drawable_obj {
public:
    Drawable_obj(const int y, const int x, const int speed) :x{x}, y{y}, speed{speed}
    {}

    virtual void draw() const = 0;

    virtual void move(const int newx, const int newy)
    {
        x = newx;
        y = newy;
    }
protected:
    int x,y,speed;
};
