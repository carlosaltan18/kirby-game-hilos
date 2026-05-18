#include "../../include/Projectile.h"

Projectile::Projectile(int x, int y) : Entity(x, y, 4, 1) {}
void Projectile::update() { x += 2; }

std::string Projectile::getSymbol() {
    return "* O~";
}