#include "../../include/Enemy.h"

Enemy::Enemy(int x, int y) : Character(x, y, 5, 1, 1) {}

void Enemy::update() {
}

bool Enemy::canBeAbsorbed() {
    return true;
}

KirbyAbility Enemy::getCopiedAbility() {
    return KirbyAbility::STAR_SHOT;
}

std::string Enemy::getCopiedAbilityName() {
    return "Estrella";
}

std::string Enemy::getSymbol() {
    return "[0_0]";
}
