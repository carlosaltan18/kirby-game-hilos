#ifndef FOOD_H
#define FOOD_H

#include "Item.h"

class Food : public Item {

public:

    Food(int x, int y);

    char getSymbol() override;
};

#endif