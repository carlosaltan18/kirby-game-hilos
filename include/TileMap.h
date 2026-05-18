#ifndef TILE_MAP_H
#define TILE_MAP_H

#include <vector>
#include <string>

class TileMap {

private:

    std::vector<std::string> mapData;

public:

    TileMap();

    void load(
        std::string filename
    );

    char getTile(
        int x,
        int y
    );

    int getWidth();

    int getHeight();
};

#endif