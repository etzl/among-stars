#ifndef OBJECTS_HP_
#define OBJECTS_HP_


#include "drawable.hpp"


constexpr float _Enemy_speed {5};
constexpr float _Player_speed {20};
constexpr int _Player_maxhealth {10};
constexpr int _Enemy_maxhealth {1};

#define _Player_shape R"(/_\)"
constexpr short _Player_shape_len {1};
constexpr short _Player_shape_col {3};
constexpr short _Player_color {5}; /* initialize at main.cpp */

#define _Enemy_shape R"(>||<)"
constexpr short _Enemy_shape_len {1};
constexpr short _Enemy_shape_col {4};
constexpr short _Enemy_color {6}; /* initialize at main.cpp */


class Bullet;

enum class Enemy_states {
    idle, high, up_left, up_right, down_left, down_right
};

class Interactable :public Drawable_obj {
    public:
        Interactable(const int h, const float y, const float x, const float speed):
            Drawable_obj(y, x, speed), health{h}
        {}

        /** Causes object to lose health point
         * \param damage The amount of health to lose
         * \return void
         */
        void hit(int damage) { health -= damage; }

        /** Get object's current health
         * \return A non-negative number indicating health
         */
        int gethealth() { return health; }

        /** Collision detection
         * \param x The position in columns of the other object
         * \return A boolean indicating collision with true, and false otherwise
         */
        virtual bool inrange(float x) const =0;

        bool operator==(const Interactable& cmp)
        {
            return (cmp.health == health) && (Drawable_obj::operator==(cmp));
        }
        bool operator!=(const Interactable& cmp)
        {
            return !(*this == cmp);
        }

    protected:
        int health;
};

class Player : public Interactable {
    public:
        Player(const float y, const float x) :Interactable(_Player_maxhealth, y, x,
            _Player_speed)
        {}

        /** Shoot a bullet off player
         * \return A rvalue-reference to the newly created bullet that should be managed by the game manager
         */
        Bullet shoot();

        /** Draw player on the scren
         * \param plac A window to draw on
         * \return void
         */
        void draw() const override
        {
            attron(COLOR_PAIR(_Player_color));
            mvaddstr(static_cast<int>(y), static_cast<int>(x-1), _Player_shape);
            attroff(COLOR_PAIR(_Player_color));
        }

        /** Vertical size, occupied by the player
         * \return A non-negative number indicating lines
         */
        size_t length() const override { return _Player_shape_len; }

        /** Horizontal size, occupied by the player
         * \return A non-negative number indicating columns
         */
        size_t width() const override { return _Player_shape_col; }

        /** Checking whether another object is colliding with current object
         * \param chkx Column position of the other object
         * \return A boolean indicating collision
         */
        bool inrange(float chkx) const override
        {
            return (chkx >= x-1) && (chkx <= x+1);
        }

        /** Allowing to set the health to any parameter
         * \return A reference to the underlying health variable
         */
        int& health_cheat()
        {
            return health;
        }

        bool isdead = false;
};


class Enemy :public Interactable {
    public:
        Enemy(const float y, const float x) :Interactable(_Enemy_maxhealth, y, x,
            _Enemy_speed)
        {}

        /** Shoot a bullet off enemy
         * \return A rvalue-reference to the newly created bullet that should be managed by the game manager
         */
        Bullet shoot();

        /** Draw player on the scren
         * \param plac A window to draw on
         * \return void
         */
        void draw() const override
        {
            attron(COLOR_PAIR(_Enemy_color));
            mvaddstr(static_cast<int>(y), static_cast<int>(x-2), _Enemy_shape);
            attroff(COLOR_PAIR(_Enemy_color));
        }

        /** Vertical size, occupied by the enemy
         * \return A non-negative number indicating lines
         */
        size_t length() const override { return _Enemy_shape_len; }

        /** Horizontal size, occupied by the enemy
         * \return A non-negative number indicating columns
         */
        size_t width() const override { return _Enemy_shape_col; }

        /** Checking whether another object is colliding with current object
         * \param chkx Column position of the other object
         * \return A boolean indicating collision
         */
        bool inrange(float chkx) const override
        {
            return ((x-2) <= chkx) && (chkx <= x+1);
        }

        /** Current mode of the enemy from: idle, high, up_right, ...
         * \return An object indicating current state
         */
        Enemy_states& mode()
        {
            return state;
        }

    private:
        Enemy_states state = Enemy_states::idle;
};

// struct Window_prop {
//     constexpr Window_prop(int y, int x, int l, int c) :y{y}, x{x}, lines{l},
//     cols{c} {}
//     int y, x, lines, cols;
// };

#endif // OBJECTS_HPP_
