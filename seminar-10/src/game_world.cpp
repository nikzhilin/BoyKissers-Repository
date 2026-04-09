#include "../inc/game_world.hpp"

#include <cmath>

namespace game {

namespace {

CollisionSummary makeNoCollisionSummary()
{
    return {false, "No collision this turn."};
}

std::string buildCollisionMessage(const GameEntity& lhs, const GameEntity& rhs)
{
    return std::string(lhs.getTypeName()) + " collided with " + rhs.getTypeName() + '.';
}

}  // namespace

GameWorld::GameWorld(unsigned int seed): rng_(seed), turnCounter_(0) {}

GameWorld::GameWorld(const std::vector<GameEntity*>& entities,
                     unsigned int seed)
    : entities_(entities), rng_(seed), turnCounter_(0)
{
}

void GameWorld::addEntity(GameEntity& entity) { entities_.push_back(&entity); }

TurnResult GameWorld::step(float dt)
{
    for(GameEntity* entity: entities_)
        if(entity != nullptr && entity->isActive())
            entity->update(dt);

    CollisionSummary collision = triggerRandomCollision();

    ++turnCounter_;
    return {turnCounter_, captureSnapshots(), collision};
}

void GameWorld::renderAll() const
{
    for(const GameEntity* entity: entities_)
        if(entity != nullptr && entity->isActive())
            entity->render();
}

CollisionSummary GameWorld::triggerCollision(std::size_t lhsIndex,
                                             std::size_t rhsIndex)
{
    if(lhsIndex >= entities_.size() || rhsIndex >= entities_.size() ||
       lhsIndex == rhsIndex)
        return makeNoCollisionSummary();

    GameEntity* lhs = entities_[lhsIndex];
    GameEntity* rhs = entities_[rhsIndex];

    if(lhs == nullptr || rhs == nullptr)
        return makeNoCollisionSummary();

    if(!lhs->isActive() || !rhs->isActive())
        return makeNoCollisionSummary();

    CollisionSummary collision = {true, buildCollisionMessage(*lhs, *rhs)};

    lhs->onCollision(*rhs);
    rhs->onCollision(*lhs);

    return collision;
}

CollisionSummary GameWorld::triggerRandomCollision()
{
    std::vector<std::size_t> activeIndices = collectActiveIndices();
    if(activeIndices.size() < 2)
        return makeNoCollisionSummary();

    std::uniform_int_distribution<std::size_t> distribution(
        0, activeIndices.size() - 1);
    std::size_t firstPosition = distribution(rng_);
    std::size_t secondPosition = distribution(rng_);
    while(secondPosition == firstPosition)
        secondPosition = distribution(rng_);

    return triggerCollision(activeIndices[firstPosition],
                            activeIndices[secondPosition]);
}

std::vector<std::size_t> GameWorld::collectActiveIndices() const
{
    std::vector<std::size_t> activeIndices;
    activeIndices.reserve(entities_.size());

    for(std::size_t index = 0; index < entities_.size(); ++index)
    {
        const GameEntity* entity = entities_[index];
        if(entity != nullptr && entity->isActive())
            activeIndices.push_back(index);
    }

    return activeIndices;
}

std::vector<EntitySnapshot> GameWorld::captureSnapshots() const
{
    std::vector<EntitySnapshot> snapshots;
    snapshots.reserve(entities_.size());

    for(const GameEntity* entity: entities_)
    {
        if(entity == nullptr)
            continue;

        Position position = entity->getPosition();
        snapshots.push_back(EntitySnapshot{
            entity->getTypeName(),
            entity->getMapSymbol(),
            static_cast<int>(std::lround(position.x)),
            static_cast<int>(std::lround(position.y)),
            entity->isActive(),
        });
    }

    return snapshots;
}

}  // namespace game
