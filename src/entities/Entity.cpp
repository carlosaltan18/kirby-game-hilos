#include "../../include/Entity.h"

Entity::Entity(int x, int y) {

    this->x = x;
    this->y = y;

    velocityX = 0;
    velocityY = 0;

    active = true;
}

int Entity::getX() {
    return x;
}

int Entity::getY() {
    return y;
}

void Entity::setX(int x) {
    this->x = x;
}

void Entity::setY(int y) {
    this->y = y;
}

bool Entity::isActive() {
    return active;
}

Entity::~Entity() {}