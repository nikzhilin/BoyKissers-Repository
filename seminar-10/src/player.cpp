#include "../inc/player.hpp"

#include "../inc/enemy.hpp"
#include "../inc/power_up.hpp"

#include <algorithm>
#include <iostream>

namespace game {

Player::Player(Position position, int health, Position velocity)
    : position_(position), health_(health), velocity_(velocity)
{
}

void Player::update(float timeDifference)
{
    position_.x += velocity_.x * timeDifference;
    position_.y += velocity_.y * timeDifference;
}

void Player::render() const
{
    std::cout << '[' << getTypeName() << "] at (" << position_.x << ", "
              << position_.y << "), hp=" << health_ << '\n';
}

void Player::onCollision(const GameEntity& other)
{
    other.collideWithPlayer(*this);
}

Position Player::getPosition() const noexcept { return position_; }

const char* Player::getTypeName() const noexcept { return "Player"; }

char Player::getMapSymbol() const noexcept { return 'P'; }

void Player::collideWithPlayer(Player& player) const { (void)player; }

void Player::collideWithEnemy(Enemy& enemy) const { enemy.bounce(); }

void Player::collideWithPowerUp(PowerUp& powerUp) const { (void)powerUp; }

int Player::getHealth() const noexcept { return health_; }

Position Player::getVelocity() const noexcept { return velocity_; }

void Player::applyDamage(int amount) noexcept
{
    if(amount < 0)
        return;

    health_ = std::max(0, health_ - amount);
}

void Player::addHealth(int amount) noexcept
{
    if(amount > 0)
        health_ += amount;
}

}  // namespace game
