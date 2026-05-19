#include "../../include/Food.h"

Food::Food(int x, int y) : Item(x, y) {}

void Food::setActive(bool state) {
    active = state; 
}

std::string Food::getSymbol() {
    return "&";
} 