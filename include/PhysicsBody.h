#ifndef PHYSICS_BODY_H
#define PHYSICS_BODY_H

class PhysicsBody {

private:

    float mass;

    float gravity;

    float friction;

    int maxFallSpeed;

public:

    PhysicsBody();

    void applyGravity(int &velocityY);

    void applyFriction(int &velocityX);

    float getMass();

    float getGravity();

    float getFriction();
};

#endif