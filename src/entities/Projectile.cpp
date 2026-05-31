#include "../../include/Projectile.h"

Projectile::Projectile(int x, int y, int direction, KirbyAbility ability)
: Character(x, y, 4, 1, 1) {
    this->direction = direction < 0 ? -1 : 1;
    this->ability = ability;
    damage = ability == KirbyAbility::FIRE ? 2 : 1;
}

void Projectile::update() {
    x += 2 * direction;
}

int Projectile::getDamage() {
    return damage;
}

KirbyAbility Projectile::getAbility() {
    return ability;
}

std::string Projectile::getSymbol() {
    if (ability == KirbyAbility::FIRE) {
        return direction > 0 ? "~F>>" : "<<F~";
    }

    return direction > 0 ? "* O~" : "~O *";
}
