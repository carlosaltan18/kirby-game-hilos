#include "../../include/InputManager.h"

#include <ncurses.h>

InputManager::InputManager() {}

int InputManager::getInput() {

    return getch();
}