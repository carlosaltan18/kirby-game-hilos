#include "MainMenu.h"
#include "InstructionScreen.h"
#include "ScoreScreen.h"
#include "PauseMenu.h"
#include "GameOverScreen.h"
#include "HUD.h"
#include <ncurses.h>
#include <string>

void MainMenu::show() {
    int choice = 0;
    int highlight = 0;
    std::string options[4] = {"Iniciar Partida", "Instrucciones", "Puntajes Destacados", "Salir"};

    InstructionScreen instructionScreen;
    ScoreScreen scoreScreen;
    PauseMenu pauseMenu;
    GameOverScreen gameOverScreen;
    HUD hud;

    while(true) {
        clear();
        mvprintw(2, 20, "=============================");
        mvprintw(3, 20, "      KIRBY CONSOLE ENGINE   ");
        mvprintw(4, 20, "=============================");

        for(int i = 0; i < 4; i++) {
            if(i == highlight) attron(A_REVERSE); 
            mvprintw(8 + (i*2), 25, "%s", options[i].c_str());
            if(i == highlight) attroff(A_REVERSE);
        }

        refresh();
        choice = getch(); 

        switch(choice) {
            case KEY_UP:
                highlight = (highlight == 0) ? 3 : highlight - 1;
                break;
            case KEY_DOWN:
                highlight = (highlight == 3) ? 0 : highlight + 1;
                break;
            case 10: // Enter
                if (highlight == 0) {
                    // --- DEMOSTRACIÓN DE JUEGO (FASE 2) ---
                    bool playing = true;
                    while(playing) {
                        clear();
                        hud.draw(3, 1500, 1); // Dibuja el HUD
                        mvprintw(10, 20, "[ SIMULACION DEL JUEGO ]");
                        mvprintw(12, 10, "Presiona [ESC] para Pausar o [Q] para simular Game Over.");
                        mvprintw(15, 20, "         K       * E      ");
                        mvprintw(16, 20, "############################");
                        refresh();

                        int gameInput = getch();
                        if (gameInput == 27) { // 27 es el código ASCII de ESC
                            bool resume = pauseMenu.show();
                            if (!resume) playing = false; // Sale al menú principal
                        } else if (gameInput == 'q' || gameInput == 'Q') {
                            gameOverScreen.show(1500);
                            playing = false; // Sale al menú principal tras el Game Over
                        }
                    }
                } else if (highlight == 1) {
                    instructionScreen.show();
                } else if (highlight == 2) {
                    scoreScreen.show();       
                } else if (highlight == 3) {
                    return;                   
                }
                break;
        }
    }
}