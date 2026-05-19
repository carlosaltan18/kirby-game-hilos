#include "../../include/Boss.h"

Boss::Boss(int x, int y) : Enemy(x, y) {
    width = 7;
    height = 3;
    health = 10;
    attackTimer = 0;
}

void Boss::update() {
    attackTimer++;
    if (attackTimer % 40 == 0) {
        velocityY = -4;
    }
}

std::string Boss::getSymbol() { return "(o_o)"; }
std::string Boss::getLine1() { return "/===\\"; }
std::string Boss::getLine2() { return "( ò_ó )"; }
std::string Boss::getLine3() { return "/|_|\\"; }
