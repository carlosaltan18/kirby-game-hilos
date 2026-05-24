#include "../../include/Player.h"

Player::Player(int x, int y) : Character(x, y, 7, 1, 3) {
    score = 0;
    currentState = KirbyState::NORMAL;
    floatTimer = 0;
    facingRight = true;
}

void Player::moveLeft() {
    x -= 3;
    facingRight = false;
}

void Player::moveRight() {
    x += 3;
    facingRight = true;
}

void Player::jump() {
    if(grounded) {
        velocityY = -5; //nivel de salto normal
        grounded = false;
    } else {
        currentState = KirbyState::FLOATING;
        if (velocityY > 0) {
            velocityY = 0;
        }
        floatTimer = 40;
    }
}
void Player::setHealth(int newHealth) {
    health = newHealth;
}
void Player::inhale() { currentState = KirbyState::INHALING; }
void Player::stopInhaling() { currentState = KirbyState::NORMAL; }
void Player::addScore(int points) { score += points; }
int Player::getScore() { return score; }
bool Player::isFacingRight() { return facingRight; }
int Player::getProjectileDirection() { return facingRight ? 1 : -1; }

void Player::update() {
    // Calcula la gravedad y el movimiento vertical
    if (!grounded) {
        if (currentState == KirbyState::FLOATING) {
            velocityY = (floatTimer % 4 == 0) ? 1 : 0; // cae lento mientras flota
        } else {
            velocityY += 1; // Gravedad normal que acelera hacia abajo
            if (velocityY > 2) velocityY = 2; // Velocidad terminal
        }
    } else {
        if (velocityY > 0) velocityY = 0; // frena al tocar piso
        if (currentState == KirbyState::FLOATING) {
            currentState = KirbyState::NORMAL;
            floatTimer = 0;
        }
    }
    
    y += velocityY;

    if (currentState == KirbyState::FLOATING) {
        floatTimer--;
        if (floatTimer <= 0) currentState = KirbyState::NORMAL;
    }
}

std::string Player::getSymbol() {
    switch(currentState) {
        case KirbyState::NORMAL:
            return facingRight ? "(>'-')>" : "<('-'<)";
        case KirbyState::FLOATING:
            return facingRight ? "(^o^*)>" : "<(*^o^)";
        case KirbyState::INHALING:
            return facingRight ? "(>'O')>" : "<('O'<)";
        default:
            return facingRight ? "(>'-')>" : "<('-'<)";
    }
}
