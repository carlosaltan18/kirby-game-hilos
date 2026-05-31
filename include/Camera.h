#ifndef CAMERA_H
#define CAMERA_H

class Camera {

private:

    int offsetX;

public:

    // Inicia la camara al comienzo del mapa.
    Camera();

    // Ajusta el desplazamiento horizontal siguiendo la posicion de Kirby.
    void update(
        int playerX
    );

    // Devuelve el desplazamiento actual para recortar el mapa visible.
    int getOffsetX();
};

#endif
