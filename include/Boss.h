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
    Boss(int x, int y);
    void update() override;
    std::string getSymbol() override;
    std::string getLine1();
    std::string getLine2();
    std::string getLine3();
};
#endif
