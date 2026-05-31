#include "../../include/InstructionScreen.h"
#include <ncurses.h>
#include <string>


void InstructionScreen::show() {
    clear(); 

    int maxY, maxX;
    getmaxyx(stdscr, maxY, maxX);


    int contentWidth = 72;

    int startX = (maxX - contentWidth) / 2;
    if (startX < 0) startX = 0; 
    
    int startY = (maxY - 23) / 2;
    if (startY < 0) startY = 0;

    if (has_colors()) attron(COLOR_PAIR(1) | A_BOLD);
    else attron(A_BOLD);
    std::string title = "INSTRUCCIONES";
    int titleX = (maxX - title.length()) / 2;
    mvprintw(startY, titleX - 8, "=============================");
    mvprintw(startY + 1, titleX, "%s", title.c_str());
    mvprintw(startY + 2, titleX - 8, "=============================");
    if (has_colors()) attroff(COLOR_PAIR(1) | A_BOLD);
    else attroff(A_BOLD);

    if (has_colors()) attron(COLOR_PAIR(4) | A_BOLD | A_UNDERLINE);
    else attron(A_BOLD | A_UNDERLINE);
    mvprintw(startY + 4, startX, "OBJETIVO:");
    if (has_colors()) attroff(COLOR_PAIR(4) | A_BOLD | A_UNDERLINE);
    else attroff(A_BOLD | A_UNDERLINE);
    mvprintw(startY + 5, startX, "Avanza, evita o absorbe enemigos y llega a la meta.");
    mvprintw(startY + 6, startX, "Hay modo jugador y modo computadora automatica.");

    if (has_colors()) attron(COLOR_PAIR(4) | A_BOLD | A_UNDERLINE);
    else attron(A_BOLD | A_UNDERLINE);
    mvprintw(startY + 8, startX, "CONTROLES:");
    if (has_colors()) attroff(COLOR_PAIR(4) | A_BOLD | A_UNDERLINE);
    else attroff(A_BOLD | A_UNDERLINE);
    
    mvprintw(startY + 9, startX + 2, "[W] Saltar / Flotar");
    mvprintw(startY + 10, startX + 2, "[A/D] Mover y voltear a Kirby");
    mvprintw(startY + 11, startX + 2, "[J/H] Absorber hacia donde mira / dejar de absorber");
    mvprintw(startY + 12, startX + 2, "[K] Usar habilidad copiada: Estrella o Fuego");
    mvprintw(startY + 13, startX + 2, "[Q] Salir de la partida en curso");

    if (has_colors()) attron(COLOR_PAIR(4) | A_BOLD | A_UNDERLINE);
    else attron(A_BOLD | A_UNDERLINE);
    mvprintw(startY + 15, startX, "PANTALLAS:");
    if (has_colors()) attroff(COLOR_PAIR(4) | A_BOLD | A_UNDERLINE);
    else attroff(A_BOLD | A_UNDERLINE);

    mvprintw(startY + 16, startX + 2, "Menus: flechas ARRIBA/ABAJO y ENTER.");
    mvprintw(startY + 17, startX + 2, "Final: escribe nombre y elige R reiniciar o M menu.");

    if (has_colors()) attron(COLOR_PAIR(4) | A_BOLD | A_UNDERLINE);
    else attron(A_BOLD | A_UNDERLINE);
    mvprintw(startY + 19, startX, "ELEMENTOS VISUALES:");
    if (has_colors()) attroff(COLOR_PAIR(4) | A_BOLD | A_UNDERLINE);
    else attroff(A_BOLD | A_UNDERLINE);

    if (has_colors()) attron(COLOR_PAIR(1) | A_BOLD);
    mvprintw(startY + 20, startX + 2, "(>'-')> Kirby");
    if (has_colors()) attroff(COLOR_PAIR(1) | A_BOLD);

    if (has_colors()) attron(COLOR_PAIR(2) | A_BOLD);
    mvprintw(startY + 20, startX + 20, "[0_0] Normal  /~F~\\ Fuego");
    if (has_colors()) attroff(COLOR_PAIR(2) | A_BOLD);

    if (has_colors()) attron(COLOR_PAIR(3) | A_BOLD);
    mvprintw(startY + 21, startX + 2, "* O~ Estrella / ~F>> Fuego / (&) [+] {$} Items");
    if (has_colors()) attroff(COLOR_PAIR(3) | A_BOLD);

    if (has_colors()) attron(COLOR_PAIR(5) | A_BOLD);
    mvprintw(startY + 21, startX + 35, ">>META>> Cambio");
    if (has_colors()) attroff(COLOR_PAIR(5) | A_BOLD);

    std::string exitMsg = "Presiona cualquier tecla para volver al menu...";
    int exitX = (maxX - exitMsg.length()) / 2;
    
    if (has_colors()) attron(COLOR_PAIR(3) | A_BOLD);
    else attron(A_DIM);
    mvprintw(startY + 23, exitX, "%s", exitMsg.c_str());
    if (has_colors()) attroff(COLOR_PAIR(3) | A_BOLD);
    else attroff(A_DIM);
    refresh(); 
    getch();   
}
