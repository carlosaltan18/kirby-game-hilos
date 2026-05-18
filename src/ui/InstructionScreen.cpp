#include "InstructionScreen.h"
#include <ncurses.h>

void InstructionScreen::show() {
    clear(); 
    
    mvprintw(2, 20, "=============================");
    mvprintw(3, 20, "       INSTRUCCIONES         ");
    mvprintw(4, 20, "=============================");

    mvprintw(7, 15, "OBJETIVO:");
    mvprintw(8, 15, "Avanza por el nivel, evita o absorbe enemigos y llega a la meta.");

    mvprintw(11, 15, "CONTROLES:");
    mvprintw(12, 15, "[W] Saltar / Flotar");
    mvprintw(13, 15, "[A] Mover Izquierda");
    mvprintw(14, 15, "[D] Mover Derecha");
    mvprintw(15, 15, "[K] Absorber / Disparar");
    mvprintw(16, 15, "[ESC] Pausar juego");

    mvprintw(19, 15, "ELEMENTOS VISUALES:");
    mvprintw(20, 15, "'K' -> Kirby (Jugador)");
    mvprintw(21, 15, "'E' -> Enemigo");
    mvprintw(22, 15, "'*' -> Proyectil");
    mvprintw(23, 15, "'#' -> Plataforma");

    mvprintw(26, 20, "Presiona cualquier tecla para volver al menu...");
    
    refresh(); 
    getch();   
}