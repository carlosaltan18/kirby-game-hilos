#include "../../include/Food.h"
#include <cstdlib>

Food::Food(int x, int y) : Item(x, y) {
    width = 3;
    foodType = rand() % 3;

    if (foodType == 0) {
        healAmount = 1;
        scoreValue = 50;
    } else if (foodType == 1) {
        healAmount = 2;
        scoreValue = 75;
    } else {
        healAmount = 0;
        scoreValue = 150;
    }
}

void Food::setActive(bool state) {
    active = state;
}

int Food::getHealAmount() {
    return healAmount;
}

int Food::getScoreValue() {
    return scoreValue;
}

std::string Food::getSymbol() {
    if (foodType == 0) {
        return "(&)";
    }
    if (foodType == 1) {
        return "[+]";
    }

    return "{$}";
}
