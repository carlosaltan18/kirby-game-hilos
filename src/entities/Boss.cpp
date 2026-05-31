#include "../../include/Boss.h"

Boss::Boss(int x, int y) : Enemy(x, y) {
    width = 7;
    height = 3;
    health = 16;
    attackTimer = 0;
    moveDirection = -1;
    leftLimit = 12;
    rightLimit = 72;
}

void Boss::update() {
    attackTimer++;

    if (attackTimer % 2 == 0) {
        x += moveDirection;
    }

    if (x <= leftLimit) {
        x = leftLimit;
        moveDirection = 1;
    } else if (x >= rightLimit) {
        x = rightLimit;
        moveDirection = -1;
    }

    if (attackTimer % 36 < 10) {
        y = 15;
    } else {
        y = 16;
    }
}

bool Boss::canBeAbsorbed() {
    return false;
}

std::string Boss::getSymbol() { return "(o_o)"; }
std::string Boss::getLine1() { return "/===\\"; }
std::string Boss::getLine2() { return "( ò_ó )"; }
std::string Boss::getLine3() { return "/|_|\\"; }
