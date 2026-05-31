#ifndef FIRE_ENEMY_H
#define FIRE_ENEMY_H

#include "Enemy.h"

// Enemigo que contiene la habilidad Fuego. Al absorberlo no solo desaparece:
// entrega una habilidad temporal distinta a la estrella normal.
class FireEnemy : public Enemy {
public:
    // Crea un enemigo que visualmente representa fuego.
    FireEnemy(int x, int y);

    // Entrega la habilidad Fuego al ser absorbido.
    KirbyAbility getCopiedAbility() override;

    // Nombre mostrado para la habilidad Fuego.
    std::string getCopiedAbilityName() override;

    // Sprite ASCII distintivo del enemigo de fuego.
    std::string getSymbol() override;
};

#endif
