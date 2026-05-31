#ifndef CHARACTER_H
#define CHARACTER_H

#include "Entity.h"

// Entidad con vida y estado de suelo. Sirve para cualquier actor que pueda
// recibir dano o verse afectado por gravedad.
class Character : public Entity {
protected:
    int health;
    bool grounded;

public:

    Character(int x, int y, int width, int height, int health);

    void takeDamage(int damage);

    int getHealth();

    bool isGrounded();

    void setGrounded(bool grounded);
};

#endif
