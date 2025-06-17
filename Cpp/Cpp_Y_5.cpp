#include <iostream>
#include <string>
#include <utility>
using namespace std;

class Person {
protected:
    string name;

public:
    explicit Person(string n) : name(std::move(n)) {};

    virtual ~Person() = default;

    virtual void display() const {
        cout << name << " ";
    }

    virtual void modify(string n) {
        name = std::move(n);
    }
};

class Student : virtual public Person {
protected:
    int stuId;
    string major;

public:
    Student(const string& n, int id, string m) : Person(n), stuId(id), major(std::move(m)) {}

    ~Student() override = default;

    virtual void display(bool s) const {
        if (s)
            Person::display();
        cout << stuId << " " << major << " ";
    }

    virtual void modify(const string n, const int id, const string m) {
        Person::modify(n);
        stuId = id;
        major = m;
    }
};

class Teacher : virtual public Person {
protected:
    string course;

public:
    Teacher(const string& n, string c) : Person(n), course(std::move(c)) {};

    ~Teacher() override = default;

    virtual void display(const bool s) const {
        if (s)
            Person::display();
        cout << course << " ";
    }

    virtual void modify(const string n, const string c) {
        Person::modify(n);
        course = c;
    }
};

class Assistant final : public Student, public Teacher {
protected:
    int workHours;

public:
    Assistant(const string& n, const int id, const string& m, const string& c, const int h)
        : Person(n), Student(n, id, m), Teacher(n, c), workHours(h) {};

    ~Assistant() override = default;

    void display() const override {
        Student::display(true);
        Teacher::display(false);
        cout << workHours << " ";
    }

    void modify(const string& n, const int id, const string& m, const string& c, const int h) {
        Person::modify(n);
        stuId     = id;
        major     = m;
        course    = c;
        workHours = h;
    }
};

int main() {
    Person p("John Doe");
    p.display();
    cout << endl;
    Student s("Jane Doe", 12345, "Computer Science");
    s.display(true);
    cout << endl;
    Teacher t("Dr. Smith", "Physics");
    t.display(true);
    cout << endl;
    Assistant a("Alice", 67890, "Mathematics", "Calculus", 20);
    a.display();
    cout << endl;
    p.modify("John Smith");
    p.display();
    cout << endl;
    s.modify("Jane Smith", 54321, "Biology");
    s.display(true);
    cout << endl;
    t.modify("Dr. Johnson", "Chemistry");
    t.display(true);
    cout << endl;
    a.modify("Alice Smith", 98765, "Statistics", "Linear Algebra", 30);
    a.display();
    cout << endl;
    return 0;
}
