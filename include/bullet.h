
#ifndef BULLET_H
#define BULLET_H

#include <objects.h>

constexpr int _bulletspeed = 1; // defining anywhere else requires transferring


class Bullet : public Drawable_obj {
public:
    Bullet(int y, int x, Dir d) :Drawable_obj(y,x,_bulletspeed), direction{d}
    {}

    Bullet(const Bullet& cp) = delete;
    Bullet& operator=(const Bullet& cp) = delete;

    Bullet(Bullet&& mv) :Drawable_obj(mv.y,mv.x,mv.speed), direction{mv.direction}
    {
        mv.direction = Dir::none;
        mv.y = 0;
        mv.x = 0;
        mv.speed = 0;
    }
    Bullet& operator=(Bullet&& mv)
    {
        y = mv.y;
        x = mv.x;
        speed = mv.speed;
        direction = mv.direction;

        mv.direction = Dir::none;
        mv.y = 0;
        mv.x = 0;
        mv.speed = 0;

        return *this;
    }

    void update()
    {
        switch (direction) {
            case Dir::up:
                y -= speed;
                break;
            case Dir::down:
                y += speed;
                break;
        }
    }

    void draw() const override
    {
        mvaddch(y, x, ACS_BULLET);
    }
private:
    Dir direction;
};


#endif // BULLET_H