#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "Entity.h"

class Projectile : public Entity {

public:

    Projectile(int x, int y);

    void update() override;

    char getSymbol() override;
};

#endif