#include "../../include/AnimationSystem.h"

AnimationSystem::AnimationSystem() {
    frameCounter = 0;
}
//El metodo incrementa el contador de frames y lo reinicia después de 60 para crear un ciclo de animación.
void AnimationSystem::update() {
    frameCounter++;
    if (frameCounter > 60) {
        frameCounter = 0; 
    }
} 
// el metodo devuelve el sprite del enemigo dependiendo de si está activo o no,
//  y alterna entre dos sprites para crear una animación de movimiento.
std::string AnimationSystem::getEnemySprite(bool isActive) {
    if (!isActive) return "[x_x]"; 
    
    if (frameCounter % 30 < 15) {
        return "[0_0]";
    } else {
        return "[-_-]"; 
    }
}

std::string AnimationSystem::getItemSprite() {
    return (frameCounter % 20 < 10) ? "(&)" : " * ";
}