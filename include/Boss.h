#ifndef BOSS_H
#define BOSS_H
#include "Enemy.h"

class Boss : public Enemy {
private:
    int attackTimer;
    int moveDirection;
    int leftLimit;
    int rightLimit;
public:
    Boss(int x, int y);
    void update() override;
    std::string getSymbol() override;
    std::string getLine1();
    std::string getLine2();
    std::string getLine3();
};
#endif
