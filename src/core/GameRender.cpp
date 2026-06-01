#include "GameInternals.h"
#include <ncurses.h>
// Acciones y decisiones de la computadora.
void Game::render() {
    pthread_mutex_lock(&threadManager.gameMutex);

    std::vector<std::string> hiddenLog;
    // En modo jugador normal se oculta el registro
    const std::vector<std::string>& visibleLog = computerMode ? eventLog : hiddenLog;
    //
    renderer.render(map, camera, *player, enemies, projectiles, foods);
    hud.render(
        player,
        currentLevel,
        getActiveBoss(enemies),
        countActiveEnemies(),
        countActiveProjectiles(),
        visibleLog
    );
    refresh();

    pthread_mutex_unlock(&threadManager.gameMutex);
}
