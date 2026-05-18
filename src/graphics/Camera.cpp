#include "../../include/Camera.h"

Camera::Camera() {

    offsetX = 0;
}

//Actualiza la posición de la cámara en función de la posición del jugador
void Camera::update(
    int playerX
) {

    offsetX = playerX - 30;

    if(offsetX < 0)
        offsetX = 0;
}

int Camera::getOffsetX() {

    return offsetX;
}