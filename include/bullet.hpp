#ifndef BULLET_HP_
#define BULLET_HP_


#include <utility>

#include "drawable.hpp"


constexpr float _Bullet_speed {21};
constexpr short _Bullet_length {1};
constexpr short _Bullet_width {1};
#define _Bullet_shape ACS_BULLET


class Bullet :public Drawable_obj {
    public:
        Bullet(float y, float x, Dir d) :Drawable_obj{y, x, _Bullet_speed},
            direction{d}
        {}

        /** Move bullet through frame and detect collisions
         * \return a boolean indicating collision
         */
        bool update();

        /** Vertical size of the bullet
         * \return A non-negative number indicating lines
         */
        size_t length() const override { return _Bullet_length; }

        /** Horizontal size of the shape
         * \return A non-negative number indicating columns
         */
        size_t width() const override { return _Bullet_width; }

        /** Draw bullet on the screen
         * \param plac The window which we should draw on
         * \return void
         */
        void draw(WINDOW* plac) const override  // should later be changed to perform on stdscr
        {
            mvwaddch(plac, static_cast<int>(y), static_cast<int>(x), _Bullet_shape);
        }

    private:
        Dir direction = Dir::none;
};

#endif // BULLET_HP_
