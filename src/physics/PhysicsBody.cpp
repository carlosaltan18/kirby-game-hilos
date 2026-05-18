#include "../../include/PhysicsBody.h"

PhysicsBody::PhysicsBody() {
    mass = 1.0f;
    gravity = 1.0f;
    friction = 0.1f;
    maxFallSpeed = 3;
}

void PhysicsBody::applyGravity(int &velocityY) {
    velocityY += gravity;
    if(velocityY > maxFallSpeed) velocityY = maxFallSpeed;
}

void PhysicsBody::applyFriction(int &velocityX) {
    if(velocityX > 0) {
        velocityX -= friction;
        if(velocityX < 0) velocityX = 0;
    } else if(velocityX < 0) {
        velocityX += friction;
        if(velocityX > 0) velocityX = 0;
    }
}

void PhysicsBody::enforceBoundaries(int &x, int &y, int width, int height, int mapWidth, int mapHeight) {
    if (x < 0) x = 0;
    if (x + width >= mapWidth) x = mapWidth - width;
    
    if (y < 0) y = 0;
    // Si toca el fondo del mapa (vacío), debería morir, pero esto evita que rompa la consola
    if (y + height >= mapHeight) y = mapHeight - height;
}