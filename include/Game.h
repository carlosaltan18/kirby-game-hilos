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

#include "LevelManager.h"
#include "Food.h"

// Coordina una partida completa: entrada, fisica, IA, render y cambio de nivel.
// La idea es que Game conecte sistemas, no que cada sistema viva aqui adentro.
class Game {

private:

    bool running;

    // Nivel actual del flujo jugable: 1, 2 o pelea contra jefe.
    int currentLevel;

    // ======================================
    // ENTIDADES
    // ======================================

    // Se guardan como punteros porque las entidades se crean y desactivan durante la partida.
    Player* player;

    std::vector<Enemy*> enemies;

    std::vector<Projectile*> projectiles;

    std::vector<Food*> foods;

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
