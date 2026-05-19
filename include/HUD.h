#ifndef HUD_H
#define HUD_H

#include "Player.h"
#include "Boss.h"

class HUD {

public:

    HUD();

    void render(
        Player* player,
        int currentLevel,
        Boss* boss = nullptr
    );
};

#endif
