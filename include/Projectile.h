#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "Character.h"
#include <string>

class Projectile : public Character {
private:
    int direction;

public:
    Projectile(int x, int y, int direction = 1);
    void update() override;

    std::string getSymbol() override;
};

#endif
