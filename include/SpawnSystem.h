#ifndef SPAWN_SYSTEM_H
#define SPAWN_SYSTEM_H

#include <vector>

#include "Enemy.h"
#include "Food.h"

// Crea entidades nuevas para el nivel. Game decide cuando llamarlo y luego
// acomoda las comidas sobre suelo real.
class SpawnSystem {

public:

    // Agrega enemigos normales o de fuego al vector recibido.
    void spawnEnemies(
        std::vector<Enemy*> &enemies,
        int amount
    );

    // Agrega items aleatorios al vector recibido.
    void spawnFood(
        std::vector<Food*> &foods,
        int amount
    );
};

#endif
