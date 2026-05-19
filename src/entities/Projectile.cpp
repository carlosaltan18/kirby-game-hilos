#include "../../include/Projectile.h"

Projectile::Projectile(int x, int y) : Character(x, y, 4, 1, 1) {}

void Projectile::update() { 
    x += 2; 
}

std::string Projectile::getSymbol() {
    return "* O~";
}