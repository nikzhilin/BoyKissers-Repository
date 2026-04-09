#include "../inc/enemy.hpp"

#include "../inc/player.hpp"
#include "../inc/power_up.hpp"

#include <iostream>

namespace game {

Enemy::Enemy(Position position, int damage, Position direction)
    : position_(position), damage_(damage), direction_(direction)
{
}

void Enemy::update(float dt)
{
    position_.x += direction_.x * dt;
    position_.y += direction_.y * dt;
}

void Enemy::render() const
{
    std::cout << '[' << getTypeName() << "] at (" << position_.x << ", "
              << position_.y << "), damage=" << damage_ << '\n';
}

void Enemy::onCollision(const GameEntity& other)
{
    other.collideWithEnemy(*this);
}

Position Enemy::getPosition() const noexcept { return position_; }

const char* Enemy::getTypeName() const noexcept { return "Enemy"; }

char Enemy::getMapSymbol() const noexcept { return 'E'; }

void Enemy::collideWithPlayer(Player& player) const
{
    player.applyDamage(damage_);
}

void Enemy::collideWithEnemy(Enemy& enemy) const { enemy.bounce(); }

void Enemy::collideWithPowerUp(PowerUp& powerUp) const { (void)powerUp; }

int Enemy::getDamage() const noexcept { return damage_; }

Position Enemy::getDirection() const noexcept { return direction_; }

void Enemy::bounce() noexcept
{
    direction_.x = -direction_.x;
    direction_.y = -direction_.y;
}

}  // namespace game
