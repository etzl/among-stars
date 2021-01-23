#ifndef DRAWABLE_H
#define DRAWABLE_H



#include <curses.h>



enum class Dir { none, up, down, right, left };


class Drawable_obj {
public:
    Drawable_obj(const float y, const float x, const float speed) :x{x}, y{y}, speed{speed}
    {}

    virtual void draw(WINDOW*) const = 0;

    virtual float getx() const { return x; }
    virtual float gety() const { return y; }
    virtual float getspeed() const { return speed; }
    virtual Dir lastmove() const { return moved; }

    virtual void setx(float newx) { x = newx; }
    virtual void sety(float newy) { y = newy; }
    virtual void setspeed(float news) { speed = news; }

    virtual void move(const Dir dir)
    {
        moved = dir;

        switch (dir) {
            case Dir::up:
                y -= speed; // LINES decreases as we go up
                break;
            case Dir::down:
                y += speed;
                break;
            case Dir::right:
                x += speed;
                break;
            case Dir::left:
                x -= speed;
                break;
        }
    }

    bool operator==(const Drawable_obj& obj)
    {
        return (obj.x == x) && (obj.y == y);
    }
    bool operator!=(const Drawable_obj& obj)
    {
        return !(*this == obj);
    }
protected:
    float x,y,speed;
    Dir moved;
};

#endif // DRAWABLE_H