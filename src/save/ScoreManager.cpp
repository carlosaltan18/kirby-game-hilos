#include "../../include/ScoreManager.h"
#include <algorithm>
#include <cctype>
#include <fstream>
#include <sstream>

static const char* SCORE_FILE = "assets/scores.json";
static const char* LEGACY_SCORE_FILE = "assets/scores.txt";

static std::string escapeJson(const std::string& value) {
    std::string escaped;

    for (char ch : value) {
        if (ch == '"' || ch == '\\') {
            escaped += '\\';
        }
        escaped += ch;
    }

    return escaped;
}

static std::string readWholeFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

static std::vector<std::pair<std::string, int>> loadLegacyScores() {
    std::vector<std::pair<std::string, int>> scores;
    std::ifstream file(LEGACY_SCORE_FILE);
    std::string name;
    int score;

    while (file >> name >> score) {
        scores.push_back({name, score});
    }

    return scores;
}

static std::vector<std::pair<std::string, int>> parseJsonScores(const std::string& content) {
    std::vector<std::pair<std::string, int>> scores;
    size_t position = 0;

    while (true) {
        size_t nameKey = content.find("\"name\"", position);
        if (nameKey == std::string::npos) {
            break;
        }

        size_t nameStart = content.find('"', content.find(':', nameKey) + 1);
        if (nameStart == std::string::npos) {
            break;
        }

        size_t nameEnd = content.find('"', nameStart + 1);
        if (nameEnd == std::string::npos) {
            break;
        }

        std::string name = content.substr(nameStart + 1, nameEnd - nameStart - 1);
        size_t scoreKey = content.find("\"score\"", nameEnd);
        if (scoreKey == std::string::npos) {
            break;
        }

        size_t scoreStart = content.find(':', scoreKey);
        if (scoreStart == std::string::npos) {
            break;
        }
        scoreStart++;

        while (scoreStart < content.length() && std::isspace((unsigned char)content[scoreStart])) {
            scoreStart++;
        }

        size_t scoreEnd = scoreStart;
        while (scoreEnd < content.length() && std::isdigit((unsigned char)content[scoreEnd])) {
            scoreEnd++;
        }

        if (scoreEnd > scoreStart) {
            scores.push_back({name, std::stoi(content.substr(scoreStart, scoreEnd - scoreStart))});
        }

        position = scoreEnd;
    }

    return scores;
}

static void sortAndTrim(std::vector<std::pair<std::string, int>>& scores) {
    std::sort(scores.begin(), scores.end(), [](auto &left, auto &right) {
        return left.second > right.second;
    });

    if (scores.size() > 10) {
        scores.resize(10);
    }
}

static void writeJsonScores(const std::vector<std::pair<std::string, int>>& scores) {
    std::ofstream file(SCORE_FILE);
    if (!file.is_open()) {
        return;
    }

    file << "[\n";
    for (size_t i = 0; i < scores.size(); i++) {
        file << "  { \"name\": \"" << escapeJson(scores[i].first)
             << "\", \"score\": " << scores[i].second << " }";

        if (i + 1 < scores.size()) {
            file << ",";
        }
        file << "\n";
    }
    file << "]\n";
}

void ScoreManager::saveScore(int score, const std::string& playerName) {
    auto scores = loadTopScores();
    scores.push_back({playerName, score});
    sortAndTrim(scores);
    writeJsonScores(scores);
}

std::vector<std::pair<std::string, int>> ScoreManager::loadTopScores() {
    std::vector<std::pair<std::string, int>> scores = parseJsonScores(readWholeFile(SCORE_FILE));

    if (scores.empty()) {
        scores = loadLegacyScores();
    }

    sortAndTrim(scores);
    return scores;
}
