#include "../../include/Item.h"

Item::Item(int x, int y) : Entity(x, y, 1, 1) {}

void Item::update() {
    // Los consumibles permanecen estáticos hasta colisionar
}

std::string Item::getSymbol() {
    return "?";
}