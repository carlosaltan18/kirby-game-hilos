#ifndef CAMERA_H
#define CAMERA_H

class Camera {

private:

    int offsetX;

public:

    Camera();

    void update(
        int playerX
    );

    int getOffsetX();
};

#endif