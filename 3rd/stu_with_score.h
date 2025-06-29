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

    // —— 成绩管理接口 ——
    // 添加一门成绩
    void addScore(const Score& sc) {
        scores.push_back(sc);
    }

    // 根据课程名删除成绩，返回是否删除成功
    bool removeScore(const std::string& courseName) {
        auto it = std::remove_if(scores.begin(), scores.end(),
            [&](const Score& s){ return s.courseName == courseName; });
        if (it == scores.end()) return false;
        scores.erase(it, scores.end());
        return true;
    }

    // 查找某门课成绩（不存在则返回 nullptr）
    Score* findScore(const std::string& courseName) {
        auto it = std::find_if(scores.begin(), scores.end(),
            [&](const Score& s){ return s.courseName == courseName; });
        return it != scores.end() ? &*it : nullptr;
    }

    // 只读访问全部成绩
    const std::vector<Score>& getScores() const {
        return scores;
    }

    // 清空所有成绩
    void clearScores() {
        scores.clear();
    }

    // 计算加权平均分（或均分），不含 GPA 自身权重
    double averageScore() const {
        if (scores.empty()) return 0.0;
        double sum = std::accumulate(scores.begin(), scores.end(), 0.0,
            [](double acc, const Score& s){ return acc + s.score; });
        return sum / scores.size();
    }

    // 计算 GPA（假设 Score::gpa 已是该课的 GPA 值）
    double calculateGPA() const {
        if (scores.empty()) return 0.0;
        double sum = std::accumulate(scores.begin(), scores.end(), 0.0,
            [](double acc, const Score& s){ return acc + s.gpa; });
        return sum / scores.size();
    }

    // 重载输出，可打印学生基础信息 + 成绩列表
    friend std::ostream& operator<<(std::ostream& os, const Stu_withScore& s) {
        // 打印基类信息
        os << static_cast<const Student&>(s) << "\nScores:\n";
        for (auto& sc : s.scores) {
            os << "  - " << sc.courseName
               << ": score=" << sc.score
               << ", gpa="   << sc.gpa << "\n";
        }
        os << "Average Score: " << s.averageScore()
           << ", GPA: " << s.calculateGPA();
        return os;
    }


};