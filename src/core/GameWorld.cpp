#include "GameInternals.h"
#include "../../include/FireEnemy.h"
#include "../../include/SpawnSystem.h"
#include <cstdlib>
#include <ncurses.h>

Enemy* createRandomEnemy(int x, int y) {
    // Uno de cada tres spawns puede dar Fuego, el resto entrega Estrella.
    if (rand() % 3 == 0) {
        return new FireEnemy(x, y);
    }

    return new Enemy(x, y);
}

void deactivateLevelEntities(
    std::vector<Enemy*> &enemies,
    std::vector<Projectile*> &projectiles,
    std::vector<Food*> &foods
) {
    // No se borran punteros aqui porque puede haber hilos terminando.
    // Desactivar es suficiente para que dejen de actualizarse/renderizarse.
    for (auto enemy : enemies) {
        if (enemy->isActive()) {
            enemy->takeDamage(enemy->getHealth());
        }
    }

    for (auto projectile : projectiles) {
        if (projectile->isActive()) {
            projectile->takeDamage(projectile->getHealth());
        }
    }

    for (auto food : foods) {
        if (food->isActive()) {
            food->setActive(false);
        }
    }
}

void showBossScreen() {
    // Pausa breve y bloqueante: separa el nivel 2 de la pelea final.
    nodelay(stdscr, FALSE);
    clear();

    int maxY, maxX;
    getmaxyx(stdscr, maxY, maxX);

    const char* title = "!!! JEFE FINAL !!!";
    const char* boss = "Preparate: el jefe aparecio";
    const char* prompt = "Presiona cualquier tecla para entrar";

    if (has_colors()) attron(COLOR_PAIR(8) | A_BOLD);
    mvprintw(maxY / 2 - 3, (maxX - 25) / 2, "=========================");
    mvprintw(maxY / 2 - 2, (maxX - 17) / 2, "%s", title);
    mvprintw(maxY / 2 - 1, (maxX - 25) / 2, "=========================");
    if (has_colors()) attroff(COLOR_PAIR(8) | A_BOLD);

    if (has_colors()) attron(COLOR_PAIR(3) | A_BOLD);
    mvprintw(maxY / 2 + 1, (maxX - 28) / 2, "%s", boss);
    if (has_colors()) attroff(COLOR_PAIR(3) | A_BOLD);

    if (has_colors()) attron(COLOR_PAIR(4));
    mvprintw(maxY / 2 + 4, (maxX - 38) / 2, "%s", prompt);
    if (has_colors()) attroff(COLOR_PAIR(4));

    refresh();
    flushinp();
    getch();
    nodelay(stdscr, TRUE);
}

static int findGroundY(TileMap &map, int x) {
    // Busca la primera celda vacia que tenga suelo justo debajo.
    for (int y = map.getHeight() - 2; y >= 0; y--) {
        if (!map.isSolid(x, y) && map.isSolid(x, y + 1)) {
            return y;
        }
    }

    return -1;
}

void placeFoodsOnGround(std::vector<Food*> &foods, TileMap &map) {
    for (auto food : foods) {
        if (!food->isActive()) {
            continue;
        }

        int x = food->getX();
        if (x >= map.getWidth() - 2) {
            x = map.getWidth() - 3;
        }
        if (x < 1) {
            x = 1;
        }

        int groundY = findGroundY(map, x);
        // Si la X inicial cae sobre un hueco, se busca suelo cercano.
        for (int offset = 1; groundY == -1 && offset < map.getWidth(); offset++) {
            if (x + offset < map.getWidth() - 2) {
                groundY = findGroundY(map, x + offset);
                if (groundY != -1) {
                    x += offset;
                    break;
                }
            }

            if (x - offset > 1) {
                groundY = findGroundY(map, x - offset);
                if (groundY != -1) {
                    x -= offset;
                    break;
                }
            }
        }

        if (groundY == -1) {
            groundY = map.getHeight() > 2 ? map.getHeight() - 2 : 1;
        }

        food->setX(x);
        food->setY(groundY);
    }
}

int countActiveFoods(std::vector<Food*> &foods) {
    int activeFoods = 0;

    for (auto food : foods) {
        if (food->isActive()) {
            activeFoods++;
        }
    }

    return activeFoods;
}

void spawnFoodsOnGround(std::vector<Food*> &foods, TileMap &map, int amount) {
    SpawnSystem spawner;
    size_t oldSize = foods.size();

    spawner.spawnFood(foods, amount);

    // Solo se reacomodan los items nuevos para no mover los ya visibles.
    for (size_t i = oldSize; i < foods.size(); i++) {
        std::vector<Food*> newFood;
        newFood.push_back(foods[i]);
        placeFoodsOnGround(newFood, map);
    }
}

Boss* getActiveBoss(std::vector<Enemy*> &enemies) {
    // El vector guarda Enemy*, asi que se identifica al jefe por dynamic_cast.
    for (auto enemy : enemies) {
        Boss* boss = dynamic_cast<Boss*>(enemy);
        if (boss != nullptr && boss->isActive()) {
            return boss;
        }
    }

    return nullptr;
}

void resetPlayerAfterFall(Player* player, int currentLevel) {
    player->takeDamage(1);
    player->setX(5);

    // La arena del jefe tiene una altura distinta a los niveles laterales.
    if (currentLevel == 3) {
        player->setY(10);
    } else {
        player->setY(6);
    }
}
