#include "../../include/Player.h"

Player::Player(int x, int y)
: Character(x,y,3)
{
    score = 0;
}

void Player::moveLeft() {
    x--;
}

void Player::moveRight() {
    x++;
}

void Player::jump() {

    if(grounded) {

        velocityY = -4;

        grounded = false;
    }
}

void Player::addScore(int points) {
    score += points;
}

int Player::getScore() {
    return score;
}

void Player::update() {

    y += velocityY;
}

char Player::getSymbol() {
    return 'K';
}