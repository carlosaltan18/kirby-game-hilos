#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "Character.h"
#include "KirbyAbility.h"
#include <string>

// Ataque disparado por Kirby. Su habilidad define sprite y dano, mientras la
// direccion permite atacar hacia donde Kirby esta mirando.
class Projectile : public Character {
private:
    int direction;
    KirbyAbility ability;
    int damage;

public:
    // Crea un ataque con direccion y habilidad asociada.
    Projectile(
        int x,
        int y,
        int direction = 1,
        KirbyAbility ability = KirbyAbility::STAR_SHOT
    );

    // Avanza horizontalmente segun su direccion.
    void update() override;

    // Dano que causa al impactar.
    int getDamage();

    // Habilidad que define sprite y dano.
    KirbyAbility getAbility();

    // Sprite ASCII del ataque.
    std::string getSymbol() override;
};

#endif
