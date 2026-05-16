#ifndef CHARACTER_H
#define CHARACTER_H

#include "Entity.h"

class Character : public Entity {

protected:
    int health;
    bool grounded;

public:

    Character(int x, int y, int health);

    void takeDamage(int damage);

    int getHealth();

    bool isGrounded();

    void setGrounded(bool grounded);
};

#endif