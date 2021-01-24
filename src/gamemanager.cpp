#include "gamemanager.h"
#include "objects.h"
#include "bullet.h"

#include <algorithm>
#include <random>


class Rand {
public:
    Rand(int low, int high) :dist{low, high} {}

    int operator()()
    {
        return dist(engine);
    }
    void seed(int s)
    {
        engine.seed(s);
    }

private:
    std::uniform_int_distribution<> dist;
    std::default_random_engine engine;
};



constexpr int _max_enemy_per_row = 4;
constexpr int _min_enemy_per_row = 1;
constexpr int _Magic_enemy = 4; /* A magic number so generation of enemies won't happen very often */
constexpr int _Points_perenemy = 1;

constexpr float _Allowed_distance_from_end = 8;
constexpr float _High_power_speed_increase = 0.2F;
constexpr float _High_distance_from_begin = 3;

constexpr float _Relative_corner = 4;   /* The distance from right or left of the screen */



void Game_manager::generate_enemies()
{
    static Rand count_rand {_min_enemy_per_row, _max_enemy_per_row};
    static Rand x_rand{0, COLS-1};
    constexpr int y = 0;


    if (enemies.size() / _Magic_enemy != 0)
        return;


    const int max_generate = count_rand();
    for (int i=0; i!=max_generate; ++i) {
        int x = x_rand();

        bool isempty =
            std::find_if(enemies.begin(), enemies.end(), [&](Enemy& en){
                return (en.gety() == y) && (en.getx() == x);
            })==enemies.end();
        if (!isempty) {
            --i;
            continue;   // find another x position
        }

        enemies.emplace_back(y, x);

        // Constructing enemy at y=0 automatically makes it high-mode
        Enemy& lastone = enemies.back();
        lastone.mode() = Enemy_states::high;
        lastone.setspeed(lastone.getspeed()+_High_power_speed_increase);
    }
}

void Game_manager::move_enemies()
{
    const static float max_y = LINES - _Allowed_distance_from_end;
    const static float relative_right = COLS-_Relative_corner;
    static Rand chose {static_cast<int>(Enemy_states::left),
     static_cast<int>(Enemy_states::right)};


    for (auto& enemy: enemies) {
        switch (enemy.mode()) {
            case Enemy_states::high:
                enemy.move(Dir::down);

                if (enemy.gety() >= max_y) {
                    enemy.setspeed(enemy.getspeed()-_High_power_speed_increase);

                    if (relative_right <= enemy.getx()) // enemy is right
                        enemy.mode() = Enemy_states::left;
                    else if (enemy.getx() <= _Relative_corner)
                        enemy.mode() = Enemy_states::right;
                    else
                        enemy.mode() = Enemy_states{chose()};
                }
                break;
            case Enemy_states::right:
                if (enemy.lastmove() == Dir::up) {
                    enemy.move(Dir::right);

                    if (relative_right <= enemy.getx())
                        enemy.mode() = Enemy_states::left;
                }
                else {
                    enemy.move(Dir::up);

                    if (enemy.gety() < _High_distance_from_begin) {
                        enemy.mode() = Enemy_states::high;
                        enemy.setspeed(enemy.getspeed()+_High_power_speed_increase);
                    }
                }
                break;
            case Enemy_states::left:
                if (enemy.lastmove() == Dir::up) {
                    enemy.move(Dir::left);

                    if (enemy.getx() <= _Relative_corner)
                        enemy.mode() = Enemy_states::right;
                }
                else {
                    enemy.move(Dir::up);

                    if (enemy.gety() < _High_distance_from_begin) {
                        enemy.mode() = Enemy_states::high;
                        enemy.setspeed(enemy.getspeed()+_High_power_speed_increase);
                    }
                }
                break;
        }
    }
}

void Game_manager::update()
{
    // update bullets position
    for (auto p = bullets.begin(); p != bullets.end();) {
        if (p->update())
            bullets.erase(p);
        else
            ++p;
    }

    // remove deads
    for (auto p = enemies.begin(); p != enemies.end();) {
        if (p->gethealth() <= 0) {
            player_points += _Points_perenemy;
            enemies.erase(p);
        }
        else
            ++p;
    }
    if (player.gethealth() <= 0) {
        // GAME OVER! implementation...
    }
}

Player Game_manager::player = Player{0, 0}; // can't be initialized before init()
std::vector<Enemy> Game_manager::enemies;
std::vector<Bullet> Game_manager::bullets;
int Game_manager::player_points = 0;
