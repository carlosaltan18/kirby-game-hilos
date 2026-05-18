#include "../../include/TileMap.h"


TileMap::TileMap() {
    width = 0;
    height = 0;
}

/**
 * Limpia por completo el mapa actual.
 */
void TileMap::clearMap() {
    grid.clear();
    width = 0;
    height = 0;
}

/**
 *Añade una nueva fila de texto al mapa.
 * 
 * Inserta un string en la parte inferior de la cuadrícula. Actualiza
 * automáticamente la altura total del mapa y ajusta la anchura si 
 * la nueva fila es más larga que las anteriores.
*/
void TileMap::addRow(const std::string& row) {
    grid.push_back(row);
    height = grid.size(); // La altura es el número total de filas
    
    // Actualizar la anchura máxima si esta fila es más ancha
    if ((int)row.length() > width) { 
        width = row.length();
    }
}

/** * 
 * Verifica si las coordenadas dadas chocan con un bloque sólido ('#') o si
 * intentan acceder fuera de los límites del mapa.
 */
bool TileMap::isSolid(int x, int y) {
    // Si la coordenada está fuera de la memoria del mapa, lo tratamos como "pared invisible"
    // Esto evita errores de "Out of Range" y que los personajes salgan del mundo.
    if (x < 0 || x >= width || y < 0 || y >= height) {
        return true;
    }
    
    // Comprobar si el carácter en esa posición representa una pared o suelo
    return grid[y][x] == '#';
}

/**
 * Se devuelve por referencia (`&`) para evitar copiar toda la matriz en la memoria 
 * cada vez que el Renderer o el sistema de físicas necesiten leerla.
*/
std::vector<std::string>& TileMap::getGrid() {
    return grid;
}

int TileMap::getWidth() { 
    return width; 
}

int TileMap::getHeight() { 
    return height; 
}