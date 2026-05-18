#include "../../include/Renderer.h"
#include "../../include/Entity.h"
#include <ncurses.h>
#include <vector>

void Renderer::drawFrame(std::vector<Entity*>& entities) {
    clear(); 
    
    // HUD
    mvprintw(1, 2, "Vida: 3   Score: 1200   Nivel: 1");
    mvprintw(2, 0, "--------------------------------------------------");

    // Entidades
    for(Entity* e : entities) {
        if(e->isActive()) {
            mvaddch(e->getY(), e->getX(), e->getSymbol());
        }
    }

    // Plataformas estáticas (Demostración)
    for(int i = 0; i < 50; i++) {
        mvaddch(20, i, '#');
    }

    refresh(); 
}