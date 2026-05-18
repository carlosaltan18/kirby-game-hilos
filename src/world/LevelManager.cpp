#include "../../include/LevelManager.h"
#include <fstream>
#include <iostream>

LevelManager::LevelManager() {
    currentLevel = 1;
    basePath = "assets/levels/"; 
}

bool LevelManager::loadLevel(int levelNumber, TileMap* map) {
    currentLevel = levelNumber;
    std::string filename;

    switch(levelNumber) {
        case 1: filename = basePath + "level1.txt"; break;
        case 2: filename = basePath + "level2.txt"; break;
        case 3: filename = basePath + "boss.txt"; break;
        default: filename = basePath + "level1.txt"; break;
    }

    std::ifstream file(filename);
    
    if (!file.is_open()) {
        return false;
    }

    map->clearMap();
    std::string line;
    
    // Lee el archivo de texto línea por línea
    while (std::getline(file, line)) {
        map->addRow(line);
    }

    file.close();
    return true;
}

int LevelManager::getCurrentLevel() {
    return currentLevel;
}