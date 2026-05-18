#include "../../include/GravitySystem.h"

void GravitySystem::applyGravity(Character* character, TileMap* map) {
    int nextY = character->getY() + 1;
    int currentX = character->getX();
    
    int charWidth = 7; 

    bool hitGround = false;
    
    for (int i = 0; i < charWidth; i++) {
        if (map->isSolid(currentX + i, nextY)) {
            hitGround = true;
            break;
        }
    }

    if (!hitGround) {
        character->setGrounded(false);
        character->setY(nextY);
    } else {
        character->setGrounded(true);
    }
}