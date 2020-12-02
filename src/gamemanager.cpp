#include "gamemanager.h"
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



constexpr int _max_enemy_per_row = 3;
constexpr int _min_enemy_per_row = 0;
// A magic number so generation of enemies won't happen very often
constexpr int _Magic_enemy_generation = 5;
constexpr int _Points_perenemy = 1;



void Game_manager::generate_enemies()
{
    static Rand count_rand {_min_enemy_per_row, _max_enemy_per_row};
    static Rand x_rand{0, COLS-1};
    constexpr int y = 0;


    if (enemies.size() % _Magic_enemy_generation != 0)
        return;


    const int max_generate = count_rand();
    for (int i=0; i!=max_generate; ++i) {
        int x = x_rand();

        bool isempty =
            std::find(enemies.begin(), enemies.end(), Enemy{y, x})!=enemies.end();
        if (!isempty)
            continue;   // find another x position

        enemies.emplace_back(y, x);
    }
}

void Game_manager::move_enemies()
{
    int index, realdistance;
    const int playerx = player.getx();
    bool alignenemy = true;

    if (std::none_of(enemies.begin(), enemies.end(), [&](const Enemy& en) {
        return en.getx() == playerx;
    })) {
        alignenemy = false;
        int lowest_dist = getmaxx(stdscr);


        for (int i=0; i!=enemies.size(); ++i) {
            const int curdistance = std::abs(enemies[i].getx() - playerx);
            if (curdistance < lowest_dist) {
                lowest_dist = curdistance;
                realdistance = enemies[i].getx() - playerx;
                index = i;
            }
        }
    }

    for (int i=0; i!=enemies.size(); ++i) {
        if (!alignenemy && i == index) {
            if (realdistance < 0)
                enemies[index].move(Dir::right, Dir::down);
            else // 0 < realdistance
                enemies[index].move(Dir::left, Dir::down);
            continue;
        }
        enemies[i].move(Dir::none, Dir::down);
    }
}

void Game_manager::update()
{
    // update bullets position
    std::remove_if(bullets.begin(), bullets.end(), [](Bullet& b){ return b.update(); });

    // check if anyone is dead
    std::remove_if(enemies.begin(), enemies.end(), [](Enemy& e){
        if (e.gethealth() <= 0) {
            Game_manager::player_points += _Points_perenemy;
            return true;
        }
        return false;
    });
    if (Game_manager::player.gethealth() <= 0) {
        // GAME OVER! implementation...
    }
}
