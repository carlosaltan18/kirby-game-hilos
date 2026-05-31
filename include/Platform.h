#ifndef PLATFORM_H
#define PLATFORM_H

class Platform {

private:

    int x;
    int y;

    int width;

public:

    // Define una plataforma horizontal simple.
    Platform(int x, int y, int width);

    // Posicion horizontal inicial de la plataforma.
    int getX();

    // Altura de la plataforma en el mapa.
    int getY();

    // Ancho disponible para apoyarse.
    int getWidth();

    // Indica si una entidad esta justo sobre esta plataforma.
    bool isOnPlatform(int entityX, int entityY);
};

#endif
