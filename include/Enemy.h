#ifndef ENEMY_H
#define ENEMY_H

#include "Character.h"

class Enemy : public Character {

private:

    int damage;

public:

    Enemy(
        int x,
        int y
    );

    void update() override;

    char getSymbol() override;

    int getDamage();
};

#endif