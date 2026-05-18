#ifndef TILE_MAP_H
#define TILE_MAP_H

#include <vector>
#include <string>

class TileMap {
private:
    std::vector<std::string> grid;
    int width;
    int height;

public:
    TileMap();
    
    void clearMap();
    void addRow(const std::string& row);
    
    bool isSolid(int x, int y);
    std::vector<std::string>& getGrid();
    
    int getWidth();
    int getHeight();
};

#endif