#include "drawable.hpp"
#include "gamemanager.hpp"


void Drawable_obj::move(const Dir dir)
{
    moved = dir;

    switch (dir) {
        case Dir::up:
            if (M_COLLISION_RANGE < y) // don't go out of screen
                y -= speed * Game_manager::deltatime; // 'LINES' decreases as we go up
            break;
        case Dir::down:
            if (y < static_cast<float>(M_MAX_COLLISION_RANGE_Y))
                y += speed * Game_manager::deltatime;
            break;
        case Dir::right:
            if (x < static_cast<float>(M_MAX_COLLISION_RANGE_X))
                x += speed * Game_manager::deltatime;
            break;
        case Dir::left:
            if (M_COLLISION_RANGE < x)
                x -= speed * Game_manager::deltatime;
            break;
        default:
            throw "Can't move to direction Dir::none";
    }
}
