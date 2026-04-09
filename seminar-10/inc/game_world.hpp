#pragma once

#include <cstddef>
#include <random>
#include <string>
#include <vector>

#include "game_entity.hpp"

namespace game {

struct EntitySnapshot {
    std::string typeName;
    char symbol;
    int x;
    int y;
    bool active;
};

struct CollisionSummary {
    bool happened;
    std::string message;
};

struct TurnResult {
    int turnNumber;
    std::vector<EntitySnapshot> snapshots;
    CollisionSummary collision;
};

class GameWorld {
public:
    explicit GameWorld(unsigned int seed = 42U);
    GameWorld(const std::vector<GameEntity*>& entities, unsigned int seed = 42U);

    void addEntity(GameEntity& entity);
    TurnResult step(float dt);
    void renderAll() const;
    CollisionSummary triggerCollision(std::size_t lhsIndex, std::size_t rhsIndex);
    CollisionSummary triggerRandomCollision();

private:
    std::vector<std::size_t> collectActiveIndices() const;
    std::vector<EntitySnapshot> captureSnapshots() const;

    std::vector<GameEntity*> entities_;
    mutable std::mt19937 rng_;
    int turnCounter_;
};

}  // namespace game
