#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "Character.h" 
#include <string>

class Projectile : public Character { 
public:
    Projectile(int x, int y);
    void update() override;
    
    std::string getSymbol() override;
};

#endif