#pragma once
#include <algorithm>
#include <numeric>
#include <ostream>
#include <string>
#include <vector>
#include "Student.h"

struct Score {
    std::string courseName;
    double score;
    double gpa;

    Score() : courseName(""), score(0), gpa(0) {}

    Score(const std::string& cn, double s, double g)
        : courseName(cn), score(s), gpa(g) {}
};

class Stu_withScore : public Student {
private:
    std::vector<Score> scores;

public:
    using Student::Student;

    ~Stu_withScore() = default;

    // scores management
    auto get_scores() const -> const std::vector<Score>& { return scores; }

    void clear_scores() { scores.clear(); }

    void add_score(const Score& sc) { scores.push_back(sc); }

    bool remove_score(const std::string& courseName) {
        auto it = std::remove_if(scores.begin(), scores.end(),
                                 [&] (const Score& s) { return s.courseName == courseName; });
        if (it == scores.end()) return false;
        scores.erase(it, scores.end());
        return true;
    }

    Score* find_score(const std::string& courseName) {
        auto it = std::find_if(scores.begin(), scores.end(),
                               [&] (const Score& s) { return s.courseName == courseName; });
        return it != scores.end() ? &*it : nullptr;
    }

    double calculate_gpa() const {
        if (scores.empty()) return 0.0;
        double sum = std::accumulate(scores.begin(), scores.end(), 0.0,
                                     [] (double acc, const Score& s) { return acc + s.gpa; });
        return sum / scores.size();
    }

    friend std::ostream& operator<<(std::ostream& os, const Stu_withScore& s) {
        os << static_cast<const Student&>(s) << "\n分数:\n";
        for (auto& sc : s.scores) {
            os << "  - " << sc.courseName
                    << ": 原始分: " << sc.score
                    << ", 绩点: " << sc.gpa << "\n";
        }
        os << ", 总绩点: " << s.calculate_gpa();
        return os;
    }
};
