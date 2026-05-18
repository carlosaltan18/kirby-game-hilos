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
    
    // Nuevo: Evita que las entidades salgan del mapa
    void enforceBoundaries(int &x, int &y, int width, int height, int mapWidth, int mapHeight);

    float getMass();
    float getGravity();
    float getFriction();
};

#endif