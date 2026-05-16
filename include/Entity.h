#ifndef ENTITY_H
#define ENTITY_H

class Entity {

protected:
    int x;
    int y;

    int velocityX;
    int velocityY;

    bool active;

public:

    Entity(int x, int y);

    virtual void update() = 0;

    virtual char getSymbol() = 0;

    int getX();
    int getY();

    void setX(int x);
    void setY(int y);

    bool isActive();

    virtual ~Entity();
};

#endif