#ifndef PATH_SYSTEM_H
#define PATH_SYSTEM_H

#include "Enemy.h"
#include "Player.h"

// Rutinas de movimiento para enemigos normales. Mantiene memoria de patrulla
// por enemigo en el .cpp para que cada uno tenga su propio recorrido.
class PathSystem {

public:

    // Constructor sin estado publico; el .cpp guarda memoria por enemigo.
    PathSystem();

    // Mueve al enemigo horizontalmente hacia una X objetivo.
    void moveTowardsTarget(
        Enemy* enemy,
        int targetX
    );

    // Hace patrullar al enemigo entre dos limites.
    void patrolMovement(
        Enemy* enemy,
        int leftLimit,
        int rightLimit
    );

    // Calcula distancia horizontal aproximada entre enemigo y jugador.
    int calculateDistance(
        Enemy* enemy,
        Player* player
    );
};

#endif
