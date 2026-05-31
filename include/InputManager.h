#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

class InputManager {

public:

    // Constructor liviano; ncurses se configura desde main/Game.
    InputManager();

    // Lee una tecla sin bloquear cuando nodelay esta activo.
    int getInput();
};

#endif
