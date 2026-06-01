#include "../../include/FireEnemy.h"

FireEnemy::FireEnemy(int x, int y) : Enemy(x, y) {
    width = 5;
    height = 1;
    health = 1;
}

KirbyAbility FireEnemy::getCopiedAbility() {
    return KirbyAbility::FIRE;
}

std::string FireEnemy::getCopiedAbilityName() {
    return "Fuego";
}

std::string FireEnemy::getSymbol() {
    return "(>#<)";
}
