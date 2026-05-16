#include "../../include/PhysicsBody.h"

// ======================================
// Constructor
// ======================================

PhysicsBody::PhysicsBody() {

    mass = 1.0f;

    gravity = 1.0f;

    friction = 0.1f;

    maxFallSpeed = 3;
}

// ======================================
// Aplicar gravedad
// ======================================

void PhysicsBody::applyGravity(int &velocityY) {

    velocityY += gravity;

    if(velocityY > maxFallSpeed)
        velocityY = maxFallSpeed;
}

// ======================================
// Aplicar fricción horizontal
// ======================================

void PhysicsBody::applyFriction(int &velocityX) {

    if(velocityX > 0) {

        velocityX -= friction;

        if(velocityX < 0)
            velocityX = 0;
    }

    else if(velocityX < 0) {

        velocityX += friction;

        if(velocityX > 0)
            velocityX = 0;
    }
}

// ======================================
// Getters
// ======================================

float PhysicsBody::getMass() {
    return mass;
}

float PhysicsBody::getGravity() {
    return gravity;
}

float PhysicsBody::getFriction() {
    return friction;
}