
#ifndef BULLET_H
#define BULLET_H

#include <objects.h>

constexpr int _bulletspeed = 2;
constexpr int _bulletdamage = 1;


class Bullet : public Drawable_obj {
public:
    Bullet(int y, int x, Dir d) :Drawable_obj(y,x,_bulletspeed), direction{d}
    {}

    /* returning 1 cause game manager to remove this item */
    int update()
    {
        move(Dir::none, direction);
        if (direction == Dir::up)
            for (auto& enemy: Game_manager::enemies)
                if (enemy.gety() == y)
                    if (enemy.inrange(x)) {
                        enemy.hit(_bulletdamage);
                        return 1;
                    }
        else
            if (Game_manager::player.gety() == y)
                if (Game_manager::player.inrange(x)) {
                    Game_manager::player.hit(_bulletdamage);
                    return 1;
                }

        if (y < 0 || y > maxy)
            return 1;

        return 0;
    }

    void draw() const override
    {
        mvaddch(y, x, ACS_BULLET);
    }
private:
    Dir direction = Dir::none;
    int maxy = getmaxy(stdscr);
};


#endif // BULLET_H