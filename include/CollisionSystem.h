#ifndef COLLISION_SYSTEM_H
#define COLLISION_SYSTEM_H

// Colisiones rectangulares simples. Es suficiente para sprites ASCII porque
// cada entidad tiene una caja definida por x, y, width y height.
class CollisionSystem {
public:
    static bool checkAABB(
        int x1, int y1, int w1, int h1,
        int x2, int y2, int w2, int h2
    );
};

#endif
