#ifndef SCORE_MANAGER_H
#define SCORE_MANAGER_H
#include <vector>
#include <string>
#include <utility>

// Lee y guarda los mejores puntajes. El archivo actual es JSON, pero tambien
// puede cargar el txt viejo como respaldo.
class ScoreManager {
public:
    // Agrega un puntaje y conserva la lista ordenada.
    static void saveScore(int score, const std::string& playerName);

    // Carga los mejores puntajes disponibles.
    static std::vector<std::pair<std::string, int>> loadTopScores();
};
#endif
