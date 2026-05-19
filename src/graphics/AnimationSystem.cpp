#include "../../include/AnimationSystem.h"

AnimationSystem::AnimationSystem() {
    frameCounter = 0;
}

void AnimationSystem::update() {
    frameCounter++;
    if (frameCounter > 60) {
        frameCounter = 0; 
    }
} 

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