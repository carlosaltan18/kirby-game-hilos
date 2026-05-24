#include "../../include/Projectile.h"

Projectile::Projectile(int x, int y, int direction) : Character(x, y, 4, 1, 1) {
    this->direction = direction < 0 ? -1 : 1;
}

void Projectile::update() {
    x += 2 * direction;
}

std::string Projectile::getSymbol() {
    return direction > 0 ? "* O~" : "~O *";
}
