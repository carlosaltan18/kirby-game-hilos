#include "../../include/TileMap.h"

TileMap::TileMap() {
    width = 0;
    height = 0;
}

void TileMap::clearMap() {
    grid.clear();
    width = 0;
    height = 0;
}

void TileMap::addRow(const std::string& row) {
    grid.push_back(row);
    height = grid.size();
    if (row.length() > width) {
        width = row.length();
    }
}

bool TileMap::isSolid(int x, int y) {

    if (x < 0 || x >= width || y < 0 || y >= height) return true;
    
    return grid[y][x] == '#';
}

std::vector<std::string>& TileMap::getGrid() {
    return grid;
}

int TileMap::getWidth() { return width; }
int TileMap::getHeight() { return height; }
