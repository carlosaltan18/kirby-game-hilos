#ifndef ENEMY_H
#define ENEMY_H

#include "Character.h"
#include "KirbyAbility.h"
#include <string>

class Enemy : public Character {
public:
    // Crea un enemigo normal de una linea y poca vida.
    Enemy(int x, int y);

    // Punto de actualizacion del hilo del enemigo.
    void update() override;

    // Indica si Kirby puede absorber este enemigo.
    virtual bool canBeAbsorbed();

    // Devuelve la habilidad que Kirby obtiene al absorberlo.
    virtual KirbyAbility getCopiedAbility();

    // Nombre legible de la habilidad copiada para HUD/registro.
    virtual std::string getCopiedAbilityName();

    // Sprite ASCII del enemigo normal.
    std::string getSymbol() override;
};

#endif
