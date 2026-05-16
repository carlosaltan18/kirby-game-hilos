#ifndef ITEM_H
#define ITEM_H

#include "Entity.h"

class Item : public Entity {

public:

    Item(int x, int y);

    void update() override;

    char getSymbol() override;
};

#endif