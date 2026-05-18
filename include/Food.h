#ifndef FOOD_H
#define FOOD_H

#include "Item.h"
#include <string>

class Food : public Item {
public:
    Food(int x, int y);
    
    std::string getSymbol() override;
};

#endif