#include "../../include/Projectile.h"

Projectile::Projectile(int x, int y)
: Entity(x,y)
{}

void Projectile::update() {

    x += 2;
}

char Projectile::getSymbol() {
    return '*';
}