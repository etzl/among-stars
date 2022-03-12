#include "drawable.hpp"
#include "gamemanager.hpp"

void Drawable_obj::move(const Dir dir)
{
    moved = dir;

    switch (dir) {
        case Dir::up:
            y -= speed * Game_manager::deltatime; // LINES decreases as we go up
            break;
        case Dir::down:
            y += speed * Game_manager::deltatime;
            break;
        case Dir::right:
            x += speed * Game_manager::deltatime;
            break;
        case Dir::left:
            x -= speed * Game_manager::deltatime;
            break;
    }
}
