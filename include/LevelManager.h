#ifndef LEVEL_MANAGER_H
#define LEVEL_MANAGER_H

#include "TileMap.h"
#include <string>

class LevelManager {
private:
    int currentLevel;
    std::string basePath; 

public:
    LevelManager();
    bool loadLevel(int levelNumber, TileMap* map);
    
    int getCurrentLevel();
};

#endif