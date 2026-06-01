#include "../../include/GravitySystem.h"
// metodo que verifica si el personaje se superpone con un bloque solido, y otro metodo que verifica 
// si hay un bloque solido debajo del personaje. Luego, el metodo applyGravity aplica la gravedad al personaje, 
// moviendolo hacia abajo si no hay suelo debajo de el, y asegurandose de que no se superponga con bloques solidos.
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
// Verifica si hay un bloque solido debajo del personaje.
static bool hasGroundBelow(Character* character, TileMap* map) {
    int footY = character->getY() + character->getHeight();

    for (int x = 0; x < character->getWidth(); x++) {
        if (map->isSolid(character->getX() + x, footY)) {
            return true;
        }
    }

    return false;
}
// metodo que aplica la gravedad al personaje, moviendolo hacia abajo si no hay suelo debajo de el
// y asegurandose de que no se superponga con bloques solidos. Si el personaje es un enemigo (ancho diferente a 7), 
// se le aplica una gravedad forzada para que caiga más rápido.
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
