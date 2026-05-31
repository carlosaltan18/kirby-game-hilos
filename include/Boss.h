#ifndef BOSS_H
#define BOSS_H
#include "Enemy.h"

// Enemigo especial de la fase final. Hereda de Enemy para compartir colisiones
// y vida, pero su movimiento se controla aparte desde el loop principal.
class Boss : public Enemy {
private:
    int attackTimer;
    int moveDirection;
    // Limites de patrulla para que el jefe presione sin salirse del escenario.
    int leftLimit;
    int rightLimit;
public:
    // Crea al jefe con vida alta, tamano especial y limites de patrulla.
    Boss(int x, int y);

    // Mueve al jefe y alterna su posicion vertical de ataque.
    void update() override;

    // Impide que la pelea final termine por absorcion directa.
    bool canBeAbsorbed() override;

    // Sprite compacto usado cuando se requiere una sola linea.
    std::string getSymbol() override;

    // Primera linea del sprite grande del jefe.
    std::string getLine1();

    // Segunda linea del sprite grande del jefe.
    std::string getLine2();

    // Tercera linea del sprite grande del jefe.
    std::string getLine3();
};
#endif
