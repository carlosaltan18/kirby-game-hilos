#include "../../include/Item.h"

Item::Item(int x, int y) : Entity(x,y) {}

void Item::update() {}

std::string Item::getSymbol() {
    return "?";
}