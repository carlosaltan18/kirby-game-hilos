#include "../../include/Camera.h"

Camera::Camera() {

    offsetX = 0;
}

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