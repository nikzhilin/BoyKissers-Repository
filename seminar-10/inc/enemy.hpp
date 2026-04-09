#pragma once

#include "game_entity.hpp"

namespace game {

class Enemy : public GameEntity {
public:
    Enemy(Position position, int damage, Position direction);

    void update(float dt) override;
    void render() const override;
    void onCollision(const GameEntity& other) override;

    Position getPosition() const noexcept override;
    const char* getTypeName() const noexcept override;
    char getMapSymbol() const noexcept override;

    void collideWithPlayer(Player& player) const override;
    void collideWithEnemy(Enemy& enemy) const override;
    void collideWithPowerUp(PowerUp& powerUp) const override;

    int getDamage() const noexcept;
    Position getDirection() const noexcept;

    void bounce() noexcept;

private:
    Position position_;
    int damage_;
    Position direction_;
};

}  // namespace game
