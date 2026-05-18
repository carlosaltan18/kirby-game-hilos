#include "../../include/GravitySystem.h"

void GravitySystem::applyGravity(Character* character, TileMap* map) {
    int currentX = character->getX();
    int currentY = character->getY();
    int charWidth = character->getWidth(); 
    
    bool hitGround = false;
    
    // Escanea horizontalmente el hitbox del personaje en la posición Y justo debajo de él
    for (int i = 0; i < charWidth; i++) {
        if (map->isSolid(currentX + i, currentY + 1)) {
            hitGround = true; // Si al menos un bloque de su base toca suelo sólido, está apoyado.
            break;            
        }
    }

    // Actualiza el estado del personaje para que el resto del juego sepa si puede saltar o debe caer.
    character->setGrounded(hitGround);

    // este bucle empuja hacia arriba casilla por casilla hasta sacarlo a la superfici si se estanca
    while(map->isSolid(character->getX(), character->getY())) {
        character->setY(character->getY() - 1);
    }

    // Gravedad forzada SOLO para enemigos. 
    // Dependiendo del sprite, de 5 caracteres
    if (!hitGround && charWidth != 7) { 
        // Evita que los enemigos caigan infinitamente si salen del mapa.
        if (character->getY() < 22) { 
            character->setY(character->getY() + 1); 
        }
    }
}