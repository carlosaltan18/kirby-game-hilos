#include "../../include/GravitySystem.h"

void GravitySystem::applyGravity(Character* character) {

    if(!character->isGrounded()) {

        character->setY(
            character->getY() + 1
        );
    }
}