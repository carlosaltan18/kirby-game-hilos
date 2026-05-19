#ifndef SCORE_MANAGER_H
#define SCORE_MANAGER_H
#include <vector>
#include <string>

class ScoreManager {
public:
    static void saveScore(int score, const std::string& playerName);
    static std::vector<std::pair<std::string, int>> loadTopScores();
};
#endif