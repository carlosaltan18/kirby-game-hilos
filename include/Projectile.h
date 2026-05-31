#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "Character.h"
#include <string>

// Estrella disparada por Kirby. La direccion permite que salga hacia donde
// esta mirando el jugador, no solo hacia la derecha.
class Projectile : public Character {
private:
    int direction;

public:
    Projectile(int x, int y, int direction = 1);
    void update() override;

    std::string getSymbol() override;
};

#endif
