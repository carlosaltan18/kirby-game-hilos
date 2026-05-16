#include "../../include/Character.h"

Character::Character(int x, int y, int health)
: Entity(x,y)
{
    this->health = health;

    grounded = false;
}

void Character::takeDamage(int damage) {

    health -= damage;

    if(health <= 0)
        active = false;
}

int Character::getHealth() {
    return health;
}

bool Character::isGrounded() {
    return grounded;
}

void Character::setGrounded(bool grounded) {
    this->grounded = grounded;
}