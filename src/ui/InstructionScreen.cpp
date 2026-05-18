#include "../../include/InstructionScreen.h"
#include <ncurses.h>

void InstructionScreen::show() {
    clear(); 
    
    mvprintw(2, 20, "=============================");
    mvprintw(3, 20, "       INSTRUCCIONES         ");
    mvprintw(4, 20, "=============================");

    mvprintw(7, 15, "OBJETIVO:");
    mvprintw(8, 15, "Avanza por el nivel, evita o absorbe enemigos y llega a la meta.");

    // ======================================
    // CONTROLES  
    // ======================================
    mvprintw(11, 15, "CONTROLES:");
    mvprintw(12, 15, "[W] Saltar / Flotar en el aire");
    mvprintw(13, 15, "[A] Mover Izquierda");
    mvprintw(14, 15, "[D] Mover Derecha");
    mvprintw(15, 15, "[J] Absorber Enemigo cercano");  
    mvprintw(16, 15, "[K] Disparar Proyectil Estrella");
    mvprintw(17, 15, "[Q] Salir de la partida en curso");
    mvprintw(18, 15, "[ESC] Pausar juego");

    mvprintw(21, 15, "ELEMENTOS VISUALES:");
    mvprintw(22, 15, "(>'-')> -> Kirby (Jugador)");
    mvprintw(23, 15, "[0_0]   -> Enemigo Basico");
    mvprintw(24, 15, "* O~    -> Proyectil");
    mvprintw(25, 15, "####### -> Plataforma");

    mvprintw(27, 20, "Presiona cualquier tecla para volver al menu...");
    
    refresh(); 
    getch();   
}