#include "gamemanager.h"
#include "objects.h"
#include "bullet.h"

#include <algorithm>
#include <random>
#include <chrono>


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




void c_highmode(Enemy& chk)
{
    bool allow_high = chk.gety() < _High_distance_from_begin &&
        std::find_if(Game_manager::enemies.begin(), Game_manager::enemies.end(),
        [](auto& enemy){
            return enemy.mode() == Enemy_states::high;
        })==Game_manager::enemies.end();

    if (allow_high) {
        chk.mode() = Enemy_states::high;
        chk.setspeed(chk.getspeed()+_High_power_speed_increase);
    }
    else if (chk.gety() <= 0) {
        static Rand choose_down {static_cast<int>(Enemy_states::down_left),
            static_cast<int>(Enemy_states::down_right)};
        chk.mode() = Enemy_states{choose_down()};
    }
}

void Game_manager::generate_enemies()
{
    static Rand count_rand {_min_enemy_per_row, _max_enemy_per_row};
    static Rand x_rand{2, COLS-3}; // these spaces will be occupied by the shape
    constexpr int y = 0;


    if (enemies.size() / _Enemy_maximum_generate != 0)
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
        c_highmode(enemies.back());
    }
}

void Game_manager::move_enemies()
{
    const static float max_y = LINES - _Allowed_distance_from_end;
    const static float relative_right = COLS-_Relative_corner;

    static Rand choose_up {static_cast<int>(Enemy_states::up_left),
        static_cast<int>(Enemy_states::up_right)};


    for (auto& enemy: enemies) {
        switch (enemy.mode()) {
            case Enemy_states::high:
                enemy.move(Dir::down);

                if (enemy.gety() >= max_y) {
                    enemy.setspeed(enemy.getspeed()-_High_power_speed_increase);
                    enemy.mode() = Enemy_states{choose_up()};
                }
                break;
            case Enemy_states::up_right:
                if (enemy.lastmove() == Dir::up) {
                    enemy.move(Dir::right);

                    if (relative_right <= enemy.getx())
                        enemy.mode() = Enemy_states::up_left;
                }
                else {
                    enemy.move(Dir::up);
                    c_highmode(enemy);
                }
                break;
            case Enemy_states::up_left:
                if (enemy.lastmove() == Dir::up) {
                    enemy.move(Dir::left);

                    if (enemy.getx() <= _Relative_corner)
                        enemy.mode() = Enemy_states::up_right;
                }
                else {
                    enemy.move(Dir::up);
                    c_highmode(enemy);
                }
                break;
            case Enemy_states::down_right:
                if (enemy.lastmove() == Dir::down) {
                    enemy.move(Dir::right);
                    if (relative_right <= enemy.getx())
                        enemy.mode() = Enemy_states::down_left;
                }
                else {
                    enemy.move(Dir::down);

                    if (enemy.gety() >= max_y)
                        enemy.mode() = Enemy_states{choose_up()};
                }
                break;
            case Enemy_states::down_left:
                if (enemy.lastmove() == Dir::down) {
                    enemy.move(Dir::left);

                    if (enemy.getx() <= _Relative_corner)
                        enemy.mode() = Enemy_states::down_right;
                }
                else {
                    enemy.move(Dir::down);

                    if (enemy.gety() >= max_y)
                        enemy.mode() = Enemy_states{choose_up()};
                }
                break;
        }
    }
}

void Game_manager::shoot()
{
    using namespace std::chrono;

    static float counter = 0;

    counter += deltatime;
    if (counter >= _Shoot_interval.count()) {
        for (auto& enemy: enemies)
            Game_manager::bullets.push_back(enemy.shoot());
        counter = 0;
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
    // game over
    if (player.gethealth() <= 0) {
        player.isdead = true;
    }
}

void Game_manager::restart()
{
    player = {static_cast<float>(_Player_initial_y),
        static_cast<float>(_Player_initial_x)};
    player_points = 0;
    enemies.clear();
    bullets.clear();
    deltatime = 0;
}

Player Game_manager::player {0, 0}; // can't correctly initialize until main.cpp::init()
std::vector<Enemy> Game_manager::enemies;
std::vector<Bullet> Game_manager::bullets;
int Game_manager::player_points = 0;
float Game_manager::deltatime = std::chrono::duration<float>(_timeoutms).count(); // makes sense (better than 0) for the first frame
