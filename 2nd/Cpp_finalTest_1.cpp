#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Student {
    string id;
    double score;
    static int amount;

public:
    Student() : id(""), score(0.0) {
        amount++;
    }

    Student(string id, const double score) : id(std::move(id)), score(score) {
        amount++;
    }

    Student(const Student& other) : id(other.id), score(other.score) {
        amount++;
    }

    ~Student() {
        amount--;
    }

    void set_id(const string& id) {
        this->id = id;
    }

    string get_id() const {
        return id;
    }

    void set_score(const double score) {
        this->score = score;
    }

    double get_score() const {
        return score;
    }

    void modify_info(const string& newId) {
        id = newId;
    }

    void modify_info(const double newScore) {
        score = newScore;
    }

    void modify_info(const Student& other) {
        id    = other.id;
        score = other.score;
    }

    static int get_amount() {
        return amount;
    }

    static void display_amount() {
        cout << "学生总数：" << amount << "人" << endl;
    }
};

int Student::amount = 0;

int main() {
    vector<Student*> students;
    Student stu1;
    stu1.set_id("001");
    stu1.set_score(90.0);
    students.push_back(&stu1);
    Student stu2("002", 80.5);
    students.push_back(&stu2);
    Student tmp("003", 71.0);
    Student& stu3(tmp);
    students.push_back(&stu3);
    Student::display_amount();
    double sum = 0;
    for (const auto& s : students)
        sum += s->get_score();
    double avg = sum / Student::get_amount();
    ofstream outfile("result.txt");
    if (!outfile) {
        cerr << "Failed to write to file." << endl;
        return -1;
    }
    cout << "平均分：" << fixed << setprecision(2) << avg << endl;
    outfile << fixed << setprecision(2) << avg << endl;
    outfile.close();
    return 0;
}
