#ifndef ENEMY_H
#define ENEMY_H

#include "Character.h"
#include <string>

class Enemy : public Character {
public:
    Enemy(int x, int y);
    void update() override;
    
    std::string getSymbol() override;
};

#endif