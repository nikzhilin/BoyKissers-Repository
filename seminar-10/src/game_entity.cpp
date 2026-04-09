#include "../inc/game_entity.hpp"

namespace game {

GameEntity::~GameEntity() = default;

bool GameEntity::isActive() const noexcept { return true; }

}  // namespace game
