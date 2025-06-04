#include <array>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
using namespace std;

struct Student {
    string name;
    string id;
    double score{};
};

int main() {
    array<Student, 3> students;
    cout << "Please enter the name, ID, and score of 3 students:" << endl;
    for (int i = 0; i < 3; ++i) {
        cout << "Student #" << i + 1 << ":" << endl;
        cin >> students[i].name >> students[i].id >> students[i].score;
    }
    ofstream outfile("students.txt");
    if (!outfile) {
        cerr << "Failed to write to file." << endl;
        return -1;
    }
    for (const auto& s : students) {
        outfile << s.name << " " << s.id << " " << s.score << endl;
    }
    outfile.close();
    ifstream infile("students.txt");
    if (!infile) {
        cerr << "Failed to open file." << endl;
        return -1;
    }
    array<Student, 3> students_;
    Student tmp;
    for (int i = 0; i < 3; i++)
        if (infile >> tmp.name >> tmp.id >> tmp.score)
            students_[i] = tmp;
    infile.close();
    cout << left << setw(10) << "Name"
         << left << setw(10) << "ID"
         << right << setw(10) << "Score" << endl;
    for (const auto& s : students_) {
        cout << left << setw(10) << s.name
             << left << setw(10) << s.id
             << right << setw(10) << fixed << setprecision(1) << s.score << endl;
    }
    return 0;
}
