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
    // Crea un item aleatorio de curacion o puntaje.
    Food(int x, int y);

    // Activa o desactiva el item despues de recogerlo o limpiar nivel.
    void setActive(bool state);

    // Cantidad de vida que recupera al recogerlo.
    int getHealAmount();

    // Puntos que suma al score.
    int getScoreValue();

    // Sprite ASCII segun el tipo de item.
    std::string getSymbol() override;
};

#endif
