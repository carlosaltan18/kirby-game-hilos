#include "../../include/Enemy.h"

Enemy::Enemy(int x, int y)
: Character(x,y,1)
{}

void Enemy::update() {

    x--;

    if(x < 1)
        x = 100;
}

char Enemy::getSymbol() {
    return 'E';
}