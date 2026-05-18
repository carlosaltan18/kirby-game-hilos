#ifndef COLLISION_SYSTEM_H
#define COLLISION_SYSTEM_H

class CollisionSystem {
public:
    static bool checkAABB(
        int x1, int y1, int w1, int h1,
        int x2, int y2, int w2, int h2
    );
};

#endif