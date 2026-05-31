#ifndef FOOD_H
#define FOOD_H

#include "Item.h"
#include <string>

// Consumible/item del mapa. El tipo se decide al crearse y puede curar,
// dar puntos, o ambas cosas segun sus valores.
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
