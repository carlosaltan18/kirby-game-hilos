#ifndef ENEMY_H
#define ENEMY_H

#include "Character.h"

class Enemy : public Character {

public:

    Enemy(int x, int y);

    void update() override;

    char getSymbol() override;
};

#endif