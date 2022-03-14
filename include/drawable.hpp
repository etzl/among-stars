#ifndef DRAWABLE_HP_
#define DRAWABLE_HP_


#include <curses.h>


enum class Dir { none, up, down, right, left };


class Drawable_obj {
    public:
        Drawable_obj(const float y, const float x, const float speed) :x{x}, y{y},
            speed{speed}
        {}

        /** Draw object on a window
         * \param win A window to draw on
         * \return void
         */
        virtual void draw(WINDOW*) const =0;

        /** Vertical size of the shape
         * \return A non-negative integer indicating lines
         */
        virtual size_t length() const =0;

        /** Horizontal size of the shape
         * \return A non-negative integer indicating columns
         */
        virtual size_t width() const =0;

        /** Retrive last move direction
         * \return A Dir object indicating last move
         */
        virtual Dir lastmove() const { return moved; }

        /** Move the object in a direction using speed * deltatime
         * \param dir A direction to move to
         * \return void
         */
        virtual void move(const Dir dir);

        virtual float getx() const { return x; }
        virtual float gety() const { return y; }
        virtual float getspeed() const { return speed; }
        virtual void setx(float newx) { x = newx; }
        virtual void sety(float newy) { y = newy; }
        virtual void setspeed(float news) { speed = news; }

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