#ifndef HUD_H
#define HUD_H

#include "Player.h"
#include "Boss.h"
#include <string>
#include <vector>

// Capa superior de informacion durante la partida: vida, score, nivel y vida
// del jefe cuando hay uno activo.
class HUD {

public:

    // No guarda estado propio; existe para mantener una interfaz clara.
    HUD();

    // Dibuja vida, score, nivel, habilidad, jefe y registro opcional.
    void render(
        Player* player,
        int currentLevel,
        Boss* boss = nullptr,
        int activeEnemies = 0,
        int activeProjectiles = 0,
        const std::vector<std::string>& eventLog = std::vector<std::string>()
    );
};

#endif
