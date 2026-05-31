#ifndef LEVEL_MANAGER_H
#define LEVEL_MANAGER_H

#include "TileMap.h"
#include <string>

// Carga mapas desde assets/levels y los deja dentro de TileMap.
// Game solo pide el numero de nivel; esta clase resuelve el archivo.
class LevelManager {
private:
    int currentLevel;
    std::string basePath;

public:
    // Configura la ruta base de los archivos de nivel.
    LevelManager();

    // Carga el nivel solicitado dentro del TileMap recibido.
    bool loadLevel(int levelNumber, TileMap* map);

    // Devuelve el ultimo nivel cargado.
    int getCurrentLevel();
};

#endif
