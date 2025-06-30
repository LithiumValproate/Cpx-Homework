#pragma once

#include "student.h"

#include <map>

#ifdef USE_QTJSON
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QString>
#endif

struct Score {
    double score;
    double gpa;
    Score() : score(0), gpa(0) {}
    Score(double s, double g) : score(s), gpa(g) {}
};

class Stu_withScore : public Student {
private:
    std::map<std::string, Score> courseScore;

public:
    Stu_withScore() = default;

    ~Stu_withScore() = default;

    double get_score(const std::string& course) const {
        auto it = courseScore.find(course);
        return it != courseScore.end() ? it->second.score : 0.0;
    }

    auto get_all_scores() const -> const std::map<std::string, Score>& {
        return courseScore;
    }

    void set_scores(const std::map<std::string, Score>& new_scores) {
        courseScore = new_scores;
    }

    void add_score(const std::string& course, Score score) {
        courseScore[course] = score;
    }

    double calculate_average() const {
        if (courseScore.empty()) return 0.0;
        double sum = 0.0;
        for (const auto& pair : courseScore) {
            sum += pair.second.score;
        }
        return sum / static_cast<double>(courseScore.size());
    }
};

// -- JSON conversions for Stu_withScore --
#ifdef USE_QTJSON

inline auto score_to_qjson(const Score& s) -> QJsonObject {
    QJsonObject obj;
    obj["score"] = s.score;
    obj["gpa"] = s.gpa;
    return obj;
}

inline auto score_from_qjson(const QJsonObject& obj) -> Score {
    Score s;
    s.score = obj["score"].toDouble();
    s.gpa = obj["gpa"].toDouble();
    return s;
}

inline auto stu_with_score_to_qjson(const Stu_withScore& stu) -> QJsonObject {
    QJsonObject obj = student_to_qjson(stu);
    QJsonObject scoresObj;
    for (const auto& pair : stu.get_all_scores())
        scoresObj[QString::fromStdString(pair.first)] = score_to_qjson(pair.second);
    obj["scores"] = scoresObj;
    return obj;
}

inline auto stu_with_score_from_qjson(const QJsonObject& obj) -> Stu_withScore {
    Student baseStudent = student_from_qjson(obj);
    Stu_withScore stu;
    static_cast<Student&>(stu) = baseStudent;
    for (const auto& course : baseStudent.get_courses())
        stu.add_course(course);
    if (obj.contains("scores")) {
        QJsonObject scoresObj = obj["scores"].toObject();
        std::map<std::string, Score> scores;
        for (auto it = scoresObj.begin(); it != scoresObj.end(); ++it) {
            scores[it.key().toStdString()] = score_from_qjson(it.value().toObject());
        }
        stu.set_scores(scores);
    }
    return stu;
}

#endif
