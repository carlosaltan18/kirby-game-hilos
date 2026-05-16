#ifndef PLATFORM_H
#define PLATFORM_H

class Platform {

private:

    int x;
    int y;

    int width;

public:

    Platform(int x, int y, int width);

    int getX();
    int getY();

    int getWidth();

    bool isOnPlatform(int entityX, int entityY);
};

#endif