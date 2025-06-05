#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Transportation {
protected:
    int speed;

public:
    explicit Transportation(int speed = 0) : speed(speed) {}

    virtual ~Transportation() = default;

    virtual void print() const = 0;
};

class Vehicle : public Transportation {
protected:
    int wheel;

public:
    Vehicle(const int speed, const int wheel = 0) : Transportation(speed), wheel(wheel) {}

    void set_wheel(const int wheel) {
        this->wheel = wheel;
    }

    void print() const override {
        cout << "Speed: " << this->speed
                << ", Wheels: " << this->wheel << endl;
    }
};

class Car final : public Vehicle {
    int passenger;

public:
    Car(const int speed, const int wheel, const int passenger = 0)
        : Vehicle(speed, wheel), passenger(passenger) {}

    void operator++() {
        passenger++;
    }

    void set_passenger(const int newPassenger) {
        this->passenger = newPassenger;
    }

    void print() const override {
        cout << "Speed: " << this->speed
                << ", Wheels: " << this->wheel
                << ", Passengers: " << this->passenger << endl;
    }
};


int main() {
    Vehicle v(100, 4);
    v.print(); // Speed:100, Wheels: 4
    Car c(100, 4, 2);
    ++c;       //测试重载前置++运算符
    c.print(); // Speed: 100, Wheels: 4, Passengers: 3
    c.set_wheel(6);
    c.set_passenger(5);
    c.print(); // Speed: 100, Wheels: 6, Passengers: 5
    Transportation* t = &c;
    t->print(); // Speed: 100, Wheels: 6, Passengers: 5
    return 0;
}
