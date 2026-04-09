#pragma once

namespace game {

class Player;
class Enemy;
class PowerUp;

struct Position {
    float x;
    float y;
};

class GameEntity {
public:
    virtual ~GameEntity();

    virtual void update(float dt) = 0;
    virtual void render() const = 0;
    virtual void onCollision(const GameEntity& other) = 0;

    virtual Position getPosition() const noexcept = 0;
    virtual const char* getTypeName() const noexcept = 0;
    virtual char getMapSymbol() const noexcept = 0;
    virtual bool isActive() const noexcept;

    virtual void collideWithPlayer(Player& player) const = 0;
    virtual void collideWithEnemy(Enemy& enemy) const = 0;
    virtual void collideWithPowerUp(PowerUp& powerUp) const = 0;
};

}  // namespace game
