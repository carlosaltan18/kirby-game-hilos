#ifndef ANIMATION_SYSTEM_H
#define ANIMATION_SYSTEM_H
#include <string> 

class AnimationSystem {
private:
    int frameCounter;
public:
    AnimationSystem();
    void update();
    std::string getEnemySprite(bool isActive);
    std::string getItemSprite();
}; 

#endif