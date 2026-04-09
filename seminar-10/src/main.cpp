#include <iostream>
#include <vector>

#include "../inc/console_view.hpp"
#include "../inc/enemy.hpp"
#include "../inc/game_world.hpp"
#include "../inc/player.hpp"
#include "../inc/power_up.hpp"

int main()
{
    game::Player knight({0.0F, 0.0F}, 120, {1.0F, 0.0F});
    game::Player ranger({-2.0F, 1.0F}, 95, {0.5F, 0.5F});
    game::Enemy goblin({3.0F, 2.0F}, 15, {-1.0F, 0.0F});
    game::Enemy slime({-4.0F, -1.0F}, 8, {0.0F, 1.0F});
    game::PowerUp potion({1.0F, 1.0F}, 20, 10.0F);
    game::PowerUp blessing({-1.0F, 3.0F}, 12, 10.0F);

    std::vector<game::GameEntity*> entities = {
        &knight, &ranger, &goblin, &slime, &potion, &blessing,
    };

    game::GameWorld world(entities, 42U);

    for(int turn = 1; turn <= 4; ++turn)
    {
        std::cout << "\n=== Turn " << turn << " ===\n";
        game::TurnResult result = world.step(1.0F);
        game::printTurnResult(std::cout, result);
    }

    return 0;
}
