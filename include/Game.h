#ifndef GAME_H
#define GAME_H

#include <pthread.h>
#include <string>
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
    bool computerMode;
    bool playerThreadActive;
    bool eventThreadActive;
    bool restartRequested;
    pthread_t playerThread;
    pthread_t eventThread;
    int actionCooldown;
    int eventSignalCounter;
    int aiDecisionTimer;
    int aiBehavior;
    int aiTargetX;

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
    std::vector<std::string> eventLog;

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

    // Entrada pthread del hilo que lee controles o ejecuta la IA de Kirby.
    static void* playerThreadEntry(void* arg);

    // Entrada pthread del hilo que despierta eventos del escenario.
    static void* eventThreadEntry(void* arg);

    // Ejecuta decisiones basicas para el modo computadora.
    void processComputerInput();

    // Intenta absorber un enemigo frente a Kirby y copiar su habilidad.
    void tryAbsorbEnemy();

    // Crea un proyectil de la habilidad activa hacia donde Kirby mira.
    void fireProjectile();

    // Agrega una linea al registro visible del modo computadora.
    void logEvent(const std::string& message);

    // Cuenta enemigos activos para HUD y respawn.
    int countActiveEnemies();

    // Cuenta proyectiles activos para HUD.
    int countActiveProjectiles();

public:

    // Crea una partida en modo jugador o computadora.
    Game(bool computerMode = false);

    // Prepara ncurses, mapa, entidades iniciales e hilos principales.
    void init();

    // Carga el mapa correspondiente a una ruta logica de nivel.
    void loadLevel(
        std::string levelPath
    );

    // Procesa entrada manual o delega en el modo computadora.
    void processInput();

    // Avanza reglas de juego: fisica, colisiones, items, jefe y niveles.
    void update();

    // Dibuja mapa, entidades y HUD.
    void render();

    // Ejecuta el loop principal hasta victoria, derrota o salida.
    bool run();

    // Informa si la partida sigue activa.
    bool isRunning();
};

#endif
