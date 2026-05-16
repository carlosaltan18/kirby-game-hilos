#ifndef PLAYER_H
#define PLAYER_H

#include "Character.h"

class Player : public Character {

private:
    int score;

public:

    Player(int x, int y);

    void moveLeft();
    void moveRight();

    void jump();

    void addScore(int points);

    int getScore();

    void update() override;

    char getSymbol() override;
};

#endif