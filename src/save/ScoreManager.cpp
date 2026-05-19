#include "../../include/ScoreManager.h"
#include <fstream>
#include <algorithm>

void ScoreManager::saveScore(int score, const std::string& playerName) {
    std::ofstream file("assets/scores.txt", std::ios::app);
    if (file.is_open()) {
        file << playerName << " " << score << "\n";
        file.close();
    }
}

std::vector<std::pair<std::string, int>> ScoreManager::loadTopScores() {
    std::vector<std::pair<std::string, int>> scores;
    std::ifstream file("assets/scores.txt");
    std::string name;
    int score;

    while (file >> name >> score) {
        scores.push_back({name, score});
    }
    file.close();

    // Ordena la lista de mayor a menor basándose en el número entero
    std::sort(scores.begin(), scores.end(), [](auto &left, auto &right) {
        return left.second > right.second;
    });

    return scores;
}