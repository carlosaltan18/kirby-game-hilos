#include "../../include/Platform.h"

Platform::Platform(int x, int y, int width) {

    this->x = x;

    this->y = y;

    this->width = width;
}

int Platform::getX() {
    return x;
}

int Platform::getY() {
    return y;
}

int Platform::getWidth() {
    return width;
}

bool Platform::isOnPlatform(int entityX, int entityY) {

    // Verifica si la entidad está encima

    return (
        entityY == y - 1 &&
        entityX >= x &&
        entityX <= x + width
    );
}