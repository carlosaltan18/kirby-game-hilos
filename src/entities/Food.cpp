#include "../../include/Food.h"

Food::Food(int x, int y)
: Item(x,y)
{}

char Food::getSymbol() {
    return '&';
}