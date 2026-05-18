#ifndef GAME_H
#define GAME_H

#include <vector>

#include "Player.h"
#include "Enemy.h"
#include "Projectile.h"

#include "Renderer.h"
#include "HUD.h"

#include "TileMap.h"
#include "Camera.h"

#include "EnemyAI.h"

#include "InputManager.h"

#include "GravitySystem.h"

#include "ThreadManager.h"

class Game {

private:

    bool running;

    int currentLevel;

    // ======================================
    // ENTIDADES
    // ======================================

    Player* player;

    std::vector<Enemy*> enemies;

    std::vector<Projectile*> projectiles;

    // ======================================
    // SISTEMAS
    // ======================================

    Renderer renderer;

    HUD hud;

    Camera camera;

    TileMap map;

    EnemyAI enemyAI;

    InputManager inputManager;

    GravitySystem gravitySystem;

    ThreadManager threadManager;

public:

    Game();

    void init();

    void loadLevel(
        std::string levelPath
    );

    void processInput();

    void update();

    void render();

    void run();

    bool isRunning();
};

#endif