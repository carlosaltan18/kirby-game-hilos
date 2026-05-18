#include "../../include/Food.h"

Food::Food(int x, int y) : Item(x,y) {}

std::string Food::getSymbol() {
    return "&";
}