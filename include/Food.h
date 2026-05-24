#ifndef FOOD_H
#define FOOD_H

#include "Item.h"
#include <string>

class Food : public Item {
private:
    int foodType;
    int healAmount;
    int scoreValue;

public:
    Food(int x, int y);
    void setActive(bool state);
    int getHealAmount();
    int getScoreValue();
    std::string getSymbol() override;
};

#endif
