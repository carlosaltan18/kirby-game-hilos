#include "../../include/Enemy.h"

Enemy::Enemy(int x, int y) : Character(x, y, 5, 1, 1) {}

void Enemy::update() {
}

std::string Enemy::getSymbol() {
    return "[0_0]";
}