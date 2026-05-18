#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include "Entity.h"

class Renderer {
public:
    void drawFrame(std::vector<Entity*>& entities);
};

#endif