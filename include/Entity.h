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
    Entity(int x, int y, int width, int height);
    
    virtual void update() = 0;
    virtual std::string getSymbol() = 0;

    int getX();
    int getY();
    int getWidth();  
    int getHeight(); 
    
    void setX(int x);
    void setY(int y);
    bool isActive();
    virtual ~Entity();
};

#endif
