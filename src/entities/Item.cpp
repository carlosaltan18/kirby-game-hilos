#include "../../include/Item.h"

Item::Item(int x, int y)
: Entity(x,y)
{}

void Item::update() {}

char Item::getSymbol() {
    return '?';
}