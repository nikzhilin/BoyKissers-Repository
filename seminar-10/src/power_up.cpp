#include "../inc/power_up.hpp"

#include "../inc/enemy.hpp"
#include "../inc/player.hpp"

#include <iostream>

namespace game {

PowerUp::PowerUp(Position position, int bonusHealth, float remainingTime)
    : position_(position), bonusHealth_(bonusHealth),
      remainingTime_(remainingTime), active_(true)
{
}

void PowerUp::update(float timeDifference)
{
    if(!active_)
        return;

    remainingTime_ -= timeDifference;
    if(remainingTime_ <= 0.0F)
    {
        remainingTime_ = 0.0F;
        active_ = false;
    }
}

void PowerUp::render() const
{
    if(!active_)
        return;

    std::cout << '[' << getTypeName() << "] at (" << position_.x << ", "
              << position_.y << "), bonus=" << bonusHealth_ << '\n';
}

void PowerUp::onCollision(const GameEntity& other)
{
    other.collideWithPowerUp(*this);
}

Position PowerUp::getPosition() const noexcept { return position_; }

const char* PowerUp::getTypeName() const noexcept { return "PowerUp"; }

char PowerUp::getMapSymbol() const noexcept { return 'B'; }

bool PowerUp::isActive() const noexcept { return active_; }

void PowerUp::collideWithPlayer(Player& player) const
{
    if(!active_)
        return;

    player.addHealth(bonusHealth_);
    deactivate();
}

void PowerUp::collideWithEnemy(Enemy& enemy) const { (void)enemy; }

void PowerUp::collideWithPowerUp(PowerUp& powerUp) const { (void)powerUp; }

int PowerUp::getBonusHealth() const noexcept { return bonusHealth_; }

float PowerUp::getRemainingTime() const noexcept { return remainingTime_; }

void PowerUp::deactivate() const noexcept
{
    active_ = false;
    remainingTime_ = 0.0F;
}

}  // namespace game
