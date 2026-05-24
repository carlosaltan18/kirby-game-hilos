#ifndef PLAYER_H
#define PLAYER_H

#include "Character.h"
#include <string>

enum class KirbyState {
    NORMAL,
    FLOATING,
    INHALING
};

class Player : public Character {
private:
    int score;
    KirbyState currentState;
    int floatTimer;
    bool facingRight;

public:
    Player(int x, int y);
    void moveLeft();
    void moveRight();
    void jump();
    void inhale();
    void stopInhaling();
    
    void addScore(int points);
    int getScore();
    bool isFacingRight();
    int getProjectileDirection();
    void setHealth(int health);
    
    void update() override;
    std::string getSymbol() override;
};

#endif
