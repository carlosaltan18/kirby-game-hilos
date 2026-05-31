#ifndef HUD_H
#define HUD_H

#include "Player.h"
#include "Boss.h"

// Capa superior de informacion durante la partida: vida, score, nivel y vida
// del jefe cuando hay uno activo.
class HUD {

public:

    HUD();

    void render(
        Player* player,
        int currentLevel,
        Boss* boss = nullptr
    );
};

#endif
