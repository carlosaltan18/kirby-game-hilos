#ifndef GAME_OVER_SCREEN_H
#define GAME_OVER_SCREEN_H

class GameOverScreen {
public:
    // Muestra derrota, guarda puntaje y devuelve si se reinicia.
    bool show(int finalScore);

    // Muestra victoria, guarda puntaje y devuelve si se reinicia.
    bool showVictory(int finalScore);
};

#endif
