#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "Entity.h"
#include <string>

class Projectile : public Entity {
public:
    Projectile(int x, int y);
    void update() override;
    
    std::string getSymbol() override;
};

#endif