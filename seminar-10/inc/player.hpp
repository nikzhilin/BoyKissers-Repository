#pragma once

#include "game_entity.hpp"

namespace game {

class Player : public GameEntity {
public:
    Player(Position position, int health, Position velocity);

    void update(float dt) override;
    void render() const override;
    void onCollision(const GameEntity& other) override;

    Position getPosition() const noexcept override;
    const char* getTypeName() const noexcept override;
    char getMapSymbol() const noexcept override;

    void collideWithPlayer(Player& player) const override;
    void collideWithEnemy(Enemy& enemy) const override;
    void collideWithPowerUp(PowerUp& powerUp) const override;

    int getHealth() const noexcept;
    Position getVelocity() const noexcept;

    void applyDamage(int amount) noexcept;
    void addHealth(int amount) noexcept;

private:
    Position position_;
    int health_;
    Position velocity_;
};

}  // namespace game
