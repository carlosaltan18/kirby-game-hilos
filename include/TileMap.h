#ifndef TILE_MAP_H
#define TILE_MAP_H

#include <vector>
#include <string>

// Representa el mapa como filas de texto. El caracter '#' es el suelo/pared
// que usan fisica y colisiones para saber que es solido.
class TileMap {
private:
    std::vector<std::string> grid;
    int width;
    int height;

public:
    // Crea un mapa vacio.
    TileMap();

    // Borra filas y reinicia dimensiones.
    void clearMap();

    // Agrega una fila de texto al mapa.
    void addRow(const std::string& row);

    // Indica si una coordenada es pared/suelo o limite del mapa.
    bool isSolid(int x, int y);

    // Devuelve la grilla completa para renderizado.
    std::vector<std::string>& getGrid();

    // Ancho del mapa cargado.
    int getWidth();

    // Alto del mapa cargado.
    int getHeight();
};

#endif
