#ifndef ENTITY_H
#define ENTITY_H

#include <string>

// Base comun para todo lo que tiene posicion, tamano y puede aparecer activo
// en el mundo. Player, enemigos, proyectiles e items parten de aqui.
class Entity {
protected:
    int x;
    int y;
    int width;
    int height;

    int velocityX;
    int velocityY;
    bool active;

public:
    // Define posicion inicial, tamano y marca la entidad como activa.
    Entity(int x, int y, int width, int height);

    // Actualiza el comportamiento propio de la entidad.
    virtual void update() = 0;

    // Devuelve el texto ASCII que representa la entidad.
    virtual std::string getSymbol() = 0;

    // Posicion horizontal en coordenadas del mapa.
    int getX();

    // Posicion vertical en coordenadas del mapa.
    int getY();

    // Ancho usado por render y colisiones.
    int getWidth();

    // Alto usado por render y colisiones.
    int getHeight();

    // Cambia la posicion horizontal.
    void setX(int x);

    // Cambia la posicion vertical.
    void setY(int y);

    // Indica si la entidad sigue participando en el juego.
    bool isActive();

    // Destructor virtual para permitir herencia segura.
    virtual ~Entity();
};

#endif
