#ifndef DRAWABLE_HP_
#define DRAWABLE_HP_



#include <curses.h>


enum class Dir { none, up, down, right, left };


class Drawable_obj {
public:
    Drawable_obj(const float y, const float x, const float speed) :x{x}, y{y},
     speed{speed}
    {}

    /* Draw on any window */
    virtual void draw(WINDOW*) const = 0;
    /* Amount of columns occupied by the shape */
    virtual size_t size() const =0;

    virtual float getx() const { return x; }
    virtual float gety() const { return y; }
    virtual float getspeed() const { return speed; }
    virtual Dir lastmove() const { return moved; }

    virtual void setx(float newx) { x = newx; }
    virtual void sety(float newy) { y = newy; }
    virtual void setspeed(float news) { speed = news; }

    virtual void move(const Dir dir);

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

#endif // DRAWABLE_HP_