#include "../../include/GravitySystem.h"

static bool overlapsSolid(Character* character, TileMap* map) {
    for (int y = 0; y < character->getHeight(); y++) {
        for (int x = 0; x < character->getWidth(); x++) {
            if (map->isSolid(character->getX() + x, character->getY() + y)) {
                return true;
            }
        }
    }

    return false;
}

static bool hasGroundBelow(Character* character, TileMap* map) {
    int footY = character->getY() + character->getHeight();

    for (int x = 0; x < character->getWidth(); x++) {
        if (map->isSolid(character->getX() + x, footY)) {
            return true;
        }
    }

    return false;
}

void GravitySystem::applyGravity(Character* character, TileMap* map) {
    while (overlapsSolid(character, map) && character->getY() > 0) {
        character->setY(character->getY() - 1);
    }

    bool hitGround = hasGroundBelow(character, map);
    character->setGrounded(hitGround);

    // Gravedad forzada SOLO para enemigos.
    if (!hitGround && character->getWidth() != 7) {
        if (character->getY() < map->getHeight() - character->getHeight() - 1) {
            character->setY(character->getY() + 1);

            while (overlapsSolid(character, map) && character->getY() > 0) {
                character->setY(character->getY() - 1);
            }
        }
    }
}
