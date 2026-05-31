#ifndef PHYSICS_BODY_H
#define PHYSICS_BODY_H

class PhysicsBody {
private:
    float mass;
    float gravity;
    float friction;
    int maxFallSpeed;

public:
    // Inicializa constantes fisicas simples.
    PhysicsBody();

    // Aumenta velocidad vertical hasta el maximo de caida.
    void applyGravity(int &velocityY);

    // Reduce velocidad horizontal simulando friccion.
    void applyFriction(int &velocityX);

    // Evita que las entidades salgan de los limites del mapa.
    void enforceBoundaries(int &x, int &y, int width, int height, int mapWidth, int mapHeight);

    // Masa configurada para calculos fisicos futuros.
    float getMass();

    // Gravedad configurada para el cuerpo.
    float getGravity();

    // Friccion configurada para el cuerpo.
    float getFriction();
};

#endif
