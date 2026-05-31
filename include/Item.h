#ifndef ITEM_H
#define ITEM_H

#include "Entity.h"
#include <string>

class Item : public Entity {
public:
    // Crea un item basico en el mapa.
    Item(int x, int y);

    // Los items no se mueven, pero mantienen la interfaz de Entity.
    void update() override;

    // Sprite generico cuando una subclase no define uno propio.
    std::string getSymbol() override;
};

#endif
