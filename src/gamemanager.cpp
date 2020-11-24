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
constexpr int _Magic_enemy_generation = 3;
constexpr int _Player_initialx = 20;
constexpr int _Player_initialy = 30;

Player Game_manager::player = Player{_Player_initialy, _Player_initialx};



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
    const int playerx = player.getx();
    if (std::none_of(enemies.begin(), enemies.end(), [&](const Enemy& en) {
        return en.getx() == playerx;
    })) {
        
    }
}
