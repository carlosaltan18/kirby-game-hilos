#include "../../include/InstructionScreen.h"
#include <ncurses.h>
#include <string>


void InstructionScreen::show() {
    clear(); 

    int maxY, maxX;
    getmaxyx(stdscr, maxY, maxX);


    int contentWidth = 66; 
    
    int startX = (maxX - contentWidth) / 2;
    if (startX < 0) startX = 0; 
    
    int startY = (maxY - 25) / 2; 
    if (startY < 0) startY = 0;

    attron(A_BOLD);
    std::string title = "INSTRUCCIONES";
    int titleX = (maxX - title.length()) / 2;
    mvprintw(startY, titleX - 8, "=============================");
    mvprintw(startY + 1, titleX, "%s", title.c_str());
    mvprintw(startY + 2, titleX - 8, "=============================");
    attroff(A_BOLD);

    attron(A_BOLD | A_UNDERLINE); 
    mvprintw(startY + 5, startX, "OBJETIVO:");
    attroff(A_BOLD | A_UNDERLINE);
    mvprintw(startY + 6, startX, "Avanza por el nivel, evita o absorbe enemigos y llega a la meta.");

    attron(A_BOLD | A_UNDERLINE);
    mvprintw(startY + 9, startX, "CONTROLES:");
    attroff(A_BOLD | A_UNDERLINE);
    
    mvprintw(startY + 11, startX + 2, "[W]   Saltar / Flotar en el aire");
    mvprintw(startY + 12, startX + 2, "[A]   Mover Izquierda");
    mvprintw(startY + 13, startX + 2, "[D]   Mover Derecha");
    mvprintw(startY + 14, startX + 2, "[J]   Absorber Enemigo cercano");  
    mvprintw(startY + 15, startX + 2, "[K]   Disparar Proyectil Estrella");
    mvprintw(startY + 16, startX + 2, "[Q]   Salir de la partida en curso");
    mvprintw(startY + 17, startX + 2, "[ESC] Pausar juego");

    attron(A_BOLD | A_UNDERLINE);
    mvprintw(startY + 20, startX, "ELEMENTOS VISUALES:");
    attroff(A_BOLD | A_UNDERLINE);
    
    mvprintw(startY + 22, startX + 2, "(>'-')> -> Kirby (Jugador)");
    mvprintw(startY + 23, startX + 2, "[0_0]   -> Enemigo Basico");
    mvprintw(startY + 24, startX + 2, "* O~    -> Proyectil");
    mvprintw(startY + 25, startX + 2, "####### -> Plataforma");

    std::string exitMsg = "Presiona cualquier tecla para volver al menu...";
    int exitX = (maxX - exitMsg.length()) / 2;
    
    attron(A_DIM);
    mvprintw(startY + 28, exitX, "%s", exitMsg.c_str());
    attroff(A_DIM);
    refresh(); 
    getch();   
}