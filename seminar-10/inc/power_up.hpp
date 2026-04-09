#pragma once

#include "game_entity.hpp"

namespace game {

class PowerUp : public GameEntity {
public:
    PowerUp(Position position, int bonusHealth, float remainingTime);

    void update(float dt) override;
    void render() const override;
    void onCollision(const GameEntity& other) override;

    Position getPosition() const noexcept override;
    const char* getTypeName() const noexcept override;
    char getMapSymbol() const noexcept override;
    bool isActive() const noexcept override;

    void collideWithPlayer(Player& player) const override;
    void collideWithEnemy(Enemy& enemy) const override;
    void collideWithPowerUp(PowerUp& powerUp) const override;

    int getBonusHealth() const noexcept;
    float getRemainingTime() const noexcept;

    void deactivate() const noexcept;

private:
    Position position_;
    int bonusHealth_;
    mutable float remainingTime_;
    mutable bool active_;
};

}  // namespace game
