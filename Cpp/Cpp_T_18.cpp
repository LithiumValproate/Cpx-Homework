#include <iostream>
#include <string>
using namespace std;

class Vehicle {
protected:
    double speed;
    double mass;
    string name;

public:
    Vehicle(double s, double m, const string& n) : speed(s), mass(m), name(n) {
        cout << "调用Vehicle类构造函数" << endl;
    }

    virtual ~Vehicle() = default;

    virtual void run() = 0;

    virtual void stop() = 0;
};

class Bicycle : virtual public Vehicle {
public:
    Bicycle(const double s, const double m)
        : Vehicle(s, m, "自行车") {
        cout << "调用Bicycle类构造函数" << endl;
    }

    ~Bicycle() override = default;

    void run() override {
        cout << name << "启动！" << endl;
        cout << "时速：" << speed << endl;
        cout << "重量：" << mass << endl;
    }

    void stop() override {
        cout << name << "停止！" << endl;
    }
};

class Motocar : virtual public Vehicle {
public:
    Motocar(const double s, const double m)
        : Vehicle(s, m, "机动车") {
        cout << "调用Motorcar类构造函数" << endl;
    }

    ~Motocar() override = default;

    void run() override {
        cout << name << "启动！" << endl;
        cout << "时速：" << speed << endl;
        cout << "重量：" << mass << endl;
    }

    void stop() override {
        cout << name << "停止！" << endl;
    }
};

class Motorcycle final : public Bicycle, public Motocar {
public:
    Motorcycle(const double s, const double m)
        : Vehicle(s, m, "摩托自行车"), Bicycle(s, m), Motocar(s, m) {
        cout << "调用Motorcycle类构造函数" << endl;
    }

    ~Motorcycle() override = default;

    void run() override {
        cout << name << "启动！" << endl;
        cout << "时速：" << speed << endl;
        cout << "重量：" << mass << endl;
    }

    void stop() override {
        cout << name << "停止！" << endl;
    }
};

void display(Vehicle* v) {
    v->run();
    v->stop();
}

int main() {
    Motorcycle m(100, 1);
    display(&m);
    cout << "---------------------\n";
    Bicycle b(40, 0.5);
    display(&b);
    return 0;
}
