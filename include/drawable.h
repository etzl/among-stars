#ifndef DRAWABLE_H
#define DRAWABLE_H



#include <curses.h>



enum class Dir {
    up = -1, down = 1,  // lines decrease when we go up
    right = 1, left = -1,
    none
};


class Drawable_obj {
public:
    Drawable_obj(const float y, const float x, const float speed) :x{x}, y{y}, speed{speed}
    {}

    virtual void draw(WINDOW*) const = 0;

    virtual float getx() const { return x; }
    virtual float gety() const { return y; }
    virtual float getspeed() const { return speed; }

    virtual void setx(float newx) { x = newx; }
    virtual void sety(float newy) { y = newy; }

    virtual void move(const Dir dirx, const Dir diry)
    {
        if (dirx == Dir::none)
            y += speed * static_cast<int>(diry);
        else if (diry == Dir::none)
            x += speed * static_cast<int>(dirx);
    }
protected:
    float x,y,speed;
};

#endif // DRAWABLE_H