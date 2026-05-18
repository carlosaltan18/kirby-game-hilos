#include "MainMenu.h"

#include "InstructionScreen.h"
#include "ScoreScreen.h"
#include "PauseMenu.h"
#include "GameOverScreen.h"

#include "../../include/HUD.h"
#include "../../include/Player.h"

#include <ncurses.h>

#include <string>

// ======================================
// SHOW MENU
// ======================================

void MainMenu::show() {

    int choice = 0;

    int highlight = 0;

    std::string options[4] = {

        "Iniciar Partida",
        "Instrucciones",
        "Puntajes Destacados",
        "Salir"
    };

    // ======================================
    // SUBMENUS
    // ======================================

    InstructionScreen instructionScreen;

    ScoreScreen scoreScreen;

    PauseMenu pauseMenu;

    GameOverScreen gameOverScreen;

    HUD hud;

    // ======================================
    // PLAYER DEMO
    // ======================================

    Player demoPlayer(10,10);

    demoPlayer.addScore(1500);

    // ======================================
    // MENU LOOP
    // ======================================

    while(true) {

        clear();

        // ==================================
        // TITULO
        // ==================================

        mvprintw(
            2,
            20,
            "============================="
        );

        mvprintw(
            3,
            20,
            "      KIRBY CONSOLE ENGINE"
        );

        mvprintw(
            4,
            20,
            "============================="
        );

        // ==================================
        // OPCIONES
        // ==================================

        for(int i = 0; i < 4; i++) {

            if(i == highlight)
                attron(A_REVERSE);

            mvprintw(
                8 + (i * 2),
                25,
                "%s",
                options[i].c_str()
            );

            if(i == highlight)
                attroff(A_REVERSE);
        }

        refresh();

        choice = getch();

        // ==================================
        // INPUT MENU
        // ==================================

        switch(choice) {

            // ==============================
            // UP
            // ==============================

            case KEY_UP:

                highlight =
                    (highlight == 0)
                    ?
                    3
                    :
                    highlight - 1;

                break;

            // ==============================
            // DOWN
            // ==============================

            case KEY_DOWN:

                highlight =
                    (highlight == 3)
                    ?
                    0
                    :
                    highlight + 1;

                break;

            // ==============================
            // ENTER
            // ==============================

            case 10:

                // ==========================
                // START GAME
                // ==========================

                if(highlight == 0) {

                    bool playing = true;

                    while(playing) {

                        clear();

                        // ==================
                        // HUD
                        // ==================

                        hud.render(
                            &demoPlayer,
                            1
                        );

                        // ==================
                        // DEMO GAME
                        // ==================

                        mvprintw(
                            10,
                            20,
                            "[ SIMULACION DEL JUEGO ]"
                        );

                        mvprintw(
                            12,
                            10,
                            "ESC = Pausa | Q = Game Over"
                        );

                        // ==================
                        // PLAYER
                        // ==================

                        mvprintw(
                            15,
                            20,
                            "K"
                        );

                        // ==================
                        // PROJECTILE
                        // ==================

                        mvprintw(
                            15,
                            30,
                            "*"
                        );

                        // ==================
                        // ENEMY
                        // ==================

                        mvprintw(
                            15,
                            40,
                            "E"
                        );

                        // ==================
                        // PLATFORM
                        // ==================

                        mvprintw(
                            16,
                            20,
                            "#########################"
                        );

                        refresh();

                        // ==================
                        // INPUT GAME
                        // ==================

                        int gameInput =
                            getch();

                        // ==================
                        // PAUSE
                        // ==================

                        if(gameInput == 27) {

                            bool resume =
                                pauseMenu.show();

                            if(!resume)
                                playing = false;
                        }

                        // ==================
                        // GAME OVER
                        // ==================

                        else if(
                            gameInput == 'q'
                            ||
                            gameInput == 'Q'
                        ) {

                            gameOverScreen.show(
                                1500
                            );

                            playing = false;
                        }
                    }
                }

                // ==========================
                // INSTRUCTIONS
                // ==========================

                else if(highlight == 1) {

                    instructionScreen.show();
                }

                // ==========================
                // SCORES
                // ==========================

                else if(highlight == 2) {

                    scoreScreen.show();
                }

                // ==========================
                // EXIT
                // ==========================

                else if(highlight == 3) {

                    return;
                }

                break;
        }
    }
}