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

    // Construye una entidad con caja fisica, vida inicial y estado de suelo.
    Character(int x, int y, int width, int height, int health);

    // Reduce vida y desactiva la entidad si llega a cero.
    void takeDamage(int damage);

    // Devuelve la vida actual.
    int getHealth();

    // Indica si el personaje esta apoyado sobre una superficie.
    bool isGrounded();

    // Actualiza el estado de apoyo usado por gravedad y salto.
    void setGrounded(bool grounded);
};

#endif
