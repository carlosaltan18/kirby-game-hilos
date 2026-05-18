#ifndef ITEM_H
#define ITEM_H

#include "Entity.h"
#include <string>

class Item : public Entity {
public:
    Item(int x, int y);
    void update() override;
    
    std::string getSymbol() override;
};

#endif