#include <catch2/catch_test_macros.hpp>

#include <iostream>
#include <sstream>
#include <string>

#include "../inc/console_view.hpp"
#include "../inc/enemy.hpp"
#include "../inc/game_world.hpp"
#include "../inc/player.hpp"
#include "../inc/power_up.hpp"

TEST_CASE("Player loses health after collision with enemy")
{
    game::Player player({0.0F, 0.0F}, 100, {0.0F, 0.0F});
    game::Enemy enemy({1.0F, 0.0F}, 15, {1.0F, 0.0F});

    player.onCollision(enemy);

    REQUIRE(player.getHealth() == 85);
}

TEST_CASE("Player gains health after collision with power-up")
{
    game::Player player({0.0F, 0.0F}, 100, {0.0F, 0.0F});
    game::PowerUp powerUp({1.0F, 0.0F}, 25, 10.0F);

    player.onCollision(powerUp);

    REQUIRE(player.getHealth() == 125);
    REQUIRE(powerUp.isActive() == false);
}

TEST_CASE("Power-up becomes inactive after colliding with player")
{
    game::Player player({0.0F, 0.0F}, 100, {0.0F, 0.0F});
    game::PowerUp powerUp({1.0F, 0.0F}, 25, 10.0F);

    player.onCollision(powerUp);

    REQUIRE(powerUp.isActive() == false);
    REQUIRE(powerUp.getRemainingTime() == 0.0F);
}

TEST_CASE("Enemy reverses direction after collision with player")
{
    game::Player player({0.0F, 0.0F}, 100, {0.0F, 0.0F});
    game::Enemy enemy({1.0F, 0.0F}, 15, {2.0F, -3.0F});

    enemy.onCollision(player);

    game::Position direction = enemy.getDirection();
    REQUIRE(direction.x == -2.0F);
    REQUIRE(direction.y == 3.0F);
}

TEST_CASE("Inactive power-up is skipped by world rendering and collision")
{
    game::Player player({0.0F, 0.0F}, 100, {0.0F, 0.0F});
    game::Enemy enemy({1.0F, 0.0F}, 15, {1.0F, 0.0F});
    game::PowerUp powerUp({2.0F, 0.0F}, 30, 10.0F);
    powerUp.deactivate();

    game::GameWorld world(7U);
    world.addEntity(player);
    world.addEntity(enemy);
    world.addEntity(powerUp);

    std::ostringstream output;
    std::streambuf* previous = std::cout.rdbuf(output.rdbuf());

    world.renderAll();
    game::CollisionSummary collision = world.triggerCollision(0, 2);

    std::cout.rdbuf(previous);

    REQUIRE(output.str().find("[PowerUp]") == std::string::npos);
    REQUIRE(player.getHealth() == 100);
    REQUIRE(collision.happened == false);
}

TEST_CASE("Render output includes type name and position")
{
    game::Player player({1.5F, -2.0F}, 90, {0.0F, 0.0F});

    std::ostringstream output;
    std::streambuf* previous = std::cout.rdbuf(output.rdbuf());

    player.render();

    std::cout.rdbuf(previous);

    const std::string rendered = output.str();
    REQUIRE(rendered.find("[Player]") != std::string::npos);
    REQUIRE(rendered.find("(1.5, -2)") != std::string::npos);
}

TEST_CASE("GameWorld triggerCollision dispatches reactions to both entities")
{
    game::Player player({0.0F, 0.0F}, 100, {0.0F, 0.0F});
    game::Enemy enemy({1.0F, 0.0F}, 15, {2.0F, 0.0F});

    std::vector<game::GameEntity*> entities = {&player, &enemy};
    game::GameWorld world(entities, 13U);

    game::CollisionSummary collision = world.triggerCollision(0, 1);

    REQUIRE(player.getHealth() == 85);
    REQUIRE(enemy.getDirection().x == -2.0F);
    REQUIRE(collision.happened == true);
    REQUIRE(collision.message.find("Player collided with Enemy") !=
            std::string::npos);
}

TEST_CASE("GameWorld step returns final turn result")
{
    game::Player player({0.0F, 0.0F}, 100, {0.0F, 0.0F});
    game::Enemy enemy({1.0F, 0.0F}, 15, {0.0F, 0.0F});

    std::vector<game::GameEntity*> entities = {&player, &enemy};
    game::GameWorld world(entities, 3U);

    game::TurnResult result = world.step(1.0F);

    REQUIRE(result.turnNumber == 1);
    REQUIRE(result.collision.happened == true);
    REQUIRE(result.collision.message.find("collided") != std::string::npos);
    REQUIRE(result.snapshots.size() == 2);
}

TEST_CASE("Console view renders board and event text")
{
    game::TurnResult result = {
        2,
        {
            {"Player", 'P', 1, 0, true},
            {"Enemy", 'E', -1, 1, true},
            {"PowerUp", 'B', 0, -1, true},
        },
        {true, "Enemy collided with Player."},
    };

    const std::string rendered = game::renderTurnResult(result);

    REQUIRE(rendered.find("Turn 2") != std::string::npos);
    REQUIRE(rendered.find("Event: Enemy collided with Player.") !=
            std::string::npos);
    REQUIRE(rendered.find('P') != std::string::npos);
    REQUIRE(rendered.find('E') != std::string::npos);
    REQUIRE(rendered.find('B') != std::string::npos);
    REQUIRE(rendered.find("\033[32mP\033[0m") != std::string::npos);
}

TEST_CASE("Console view omits inactive bonuses from board")
{
    game::TurnResult result = {
        3,
        {
            {"Player", 'P', 0, 0, true},
            {"PowerUp", 'B', 0, 0, false},
        },
        {true, "PowerUp collided with Player."},
    };

    const std::string rendered = game::renderTurnResult(result);

    REQUIRE(rendered.find("\033[33mB\033[0m") == std::string::npos);
}

TEST_CASE("Console view marks overlapping active entities with star")
{
    game::TurnResult result = {
        4,
        {
            {"Player", 'P', 0, 0, true},
            {"Enemy", 'E', 0, 0, true},
        },
        {true, "Player collided with Enemy."},
    };

    const std::string rendered = game::renderTurnResult(result);

    REQUIRE(rendered.find("\033[36m*\033[0m") != std::string::npos);
}

TEST_CASE("No collision turn renders neutral event")
{
    game::Player player({0.0F, 0.0F}, 100, {0.0F, 0.0F});
    game::GameWorld world(11U);
    world.addEntity(player);

    game::TurnResult result = world.step(1.0F);
    const std::string rendered = game::renderTurnResult(result);

    REQUIRE(result.collision.happened == false);
    REQUIRE(rendered.find("No collision this turn.") != std::string::npos);
}
