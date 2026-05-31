#ifndef PLAYER_H
#define PLAYER_H

#include "Character.h"
#include "KirbyAbility.h"
#include <string>

enum class KirbyState {
    NORMAL,
    FLOATING,
    INHALING
};

// Kirby guarda su estado de animacion y tambien hacia donde mira.
// Esa direccion se usa para absorber y disparar al lado correcto.
class Player : public Character {
private:
    int score;
    KirbyState currentState;
    KirbyAbility currentAbility;
    int floatTimer;
    int abilityTimer;
    // true = mira a la derecha, false = mira a la izquierda.
    bool facingRight;

public:
    // Crea a Kirby con vida, score y estado inicial.
    Player(int x, int y);

    // Mueve a Kirby a la izquierda y cambia su direccion.
    void moveLeft();

    // Mueve a Kirby a la derecha y cambia su direccion.
    void moveRight();

    // Salta desde el suelo o flota si ya esta en el aire.
    void jump();

    // Entra al estado visual de absorcion.
    void inhale();

    // Sale del estado visual de absorcion.
    void stopInhaling();

    // Asigna una habilidad copiada y reinicia su temporizador.
    void grantAbility(KirbyAbility ability);

    // Atajo para copiar Estrella.
    void grantStarAbility();

    // Atajo para copiar Fuego.
    void grantFireAbility();

    // Suma puntos al marcador.
    void addScore(int points);

    // Devuelve el score actual.
    int getScore();

    // Indica si Kirby mira a la derecha.
    bool isFacingRight();

    // Devuelve 1 o -1 para orientar proyectiles.
    int getProjectileDirection();

    // Habilidad activa en este momento.
    KirbyAbility getAbility();

    // Indica si existe una habilidad copiada disponible.
    bool hasSpecialAbility();

    // Nombre legible de la habilidad para el HUD.
    std::string getAbilityName();

    // Ajusta la vida directamente, usado por items.
    void setHealth(int health);

    // Actualiza flotacion, gravedad interna y duracion de habilidad.
    void update() override;

    // Sprite ASCII segun estado y direccion.
    std::string getSymbol() override;
};

#endif
