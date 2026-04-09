#pragma once

#include <iosfwd>
#include <string>

#include "game_world.hpp"

namespace game {

std::string renderTurnResult(const TurnResult& result);
void printTurnResult(std::ostream& os, const TurnResult& result);

}  // namespace game
