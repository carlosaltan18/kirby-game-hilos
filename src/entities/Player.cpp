#include "../../include/Player.h"

Player::Player(int x, int y) : Character(x, y, 7, 1, 3) {
    score = 0;
    currentState = KirbyState::NORMAL;
    floatTimer = 0;
}

void Player::moveLeft() { 
    x--; 
}
void Player::moveRight() { x++; }

void Player::jump() {
    if(grounded) {
        velocityY = -4;
        grounded = false;
    } else {
        currentState = KirbyState::FLOATING;
        velocityY = -1;
        floatTimer = 50;
    }
}

void Player::inhale() {
    currentState = KirbyState::INHALING;
}

void Player::stopInhaling() {
    currentState = KirbyState::NORMAL;
}

void Player::addScore(int points) { score += points; }
int Player::getScore() { return score; }

void Player::update() {
    if (!grounded) {
        velocityY += 1; 
        if (velocityY > 3) velocityY = 3; 
    } else {
        velocityY = 0; 
    }

    y += velocityY;
    
    if (currentState == KirbyState::FLOATING) {
        floatTimer--;
        if (floatTimer <= 0) {
            currentState = KirbyState::NORMAL;
        }
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