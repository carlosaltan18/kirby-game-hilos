#include "../../include/Player.h"

Player::Player(int x, int y) : Character(x, y, 7, 1, 3) {
    score = 0;
    currentState = KirbyState::NORMAL;
    floatTimer = 0;
}

void Player::moveLeft() { x -= 3; }
void Player::moveRight() { x += 3; }

void Player::jump() {
    if(grounded) {
        velocityY = -3; //nivel de salto normal
        grounded = false;
    } else {
        currentState = KirbyState::FLOATING;
        velocityY = -5; // impulso al inflarse
        floatTimer = 40;
    }
}

void Player::inhale() { currentState = KirbyState::INHALING; }
void Player::stopInhaling() { currentState = KirbyState::NORMAL; }
void Player::addScore(int points) { score += points; }
int Player::getScore() { return score; }

void Player::update() {
    // Calcula la gravedad y el movimiento vertical
    if (!grounded) {
        if (currentState == KirbyState::FLOATING) {
            velocityY = 1; // hace que caiga lentamente mientras flota
        } else {
            velocityY += 1; // Gravedad normal que acelera hacia abajo
            if (velocityY > 2) velocityY = 2; // Velocidad terminal
        }
    } else {
        if (velocityY > 0) velocityY = 0; // frena al tocar piso
    }
    
    y += velocityY;

    if (currentState == KirbyState::FLOATING) {
        floatTimer--;
        if (floatTimer <= 0) currentState = KirbyState::NORMAL;
    }
}

std::string Player::getSymbol() {
    switch(currentState) {
        case KirbyState::NORMAL:   return "(>'-')>";
        case KirbyState::FLOATING: return "(0)";
        case KirbyState::INHALING: return "<('-'<)";
        default:                   return "(>'-')>";
    }
}