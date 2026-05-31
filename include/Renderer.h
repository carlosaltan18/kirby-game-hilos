#ifndef RENDERER_H
#define RENDERER_H

#include <vector>

#include "TileMap.h"
#include "Camera.h"

#include "Player.h"
#include "Enemy.h"
#include "Projectile.h"
#include "Food.h"
#include "AnimationSystem.h"

// Dibuja el estado visible del juego en ncurses. No decide fisica ni reglas:
// solo traduce mapa, entidades y HUD visual a caracteres en pantalla.
class Renderer {

private:

    // Avanza sprites simples y el fondo movil sin depender del loop de Game.
    AnimationSystem animationSystem;
    int backgroundFrame;

public:

    // Inicia contadores visuales del renderer.
    Renderer();

    // Dibuja el mundo visible usando mapa, camara y entidades activas.
    void render(
        TileMap &map,
        Camera &camera,
        Player &player,
        std::vector<Enemy*> &enemies,
        std::vector<Projectile*> &projectiles,
        std::vector<Food*> &foods
    );
};

#endif
