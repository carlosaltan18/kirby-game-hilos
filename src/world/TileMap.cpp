#include "../../include/TileMap.h"

#include <fstream>

TileMap::TileMap() {}

// ======================================
// LOAD MAP
// ======================================

void TileMap::load(
    std::string filename
) {

    mapData.clear();

    std::ifstream file(
        filename
    );

    std::string line;

    while(getline(file, line)) {

        mapData.push_back(line);
    }

    file.close();
}

// ======================================
// GET TILE
// ======================================

char TileMap::getTile(
    int x,
    int y
) {

    if(y < 0 || y >= (int)mapData.size())
        return ' ';

    if(x < 0 || x >= (int)mapData[y].size())
        return ' ';

    return mapData[y][x];
}

// ======================================
// WIDTH
// ======================================

int TileMap::getWidth() {

    if(mapData.empty())
        return 0;

    return mapData[0].size();
}

// ======================================
// HEIGHT
// ======================================

int TileMap::getHeight() {

    return mapData.size();
}