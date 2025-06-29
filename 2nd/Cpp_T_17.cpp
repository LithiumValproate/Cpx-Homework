#include  <iostream>
#include <string>
#include <utility>
using namespace std;

template<typename T> class Compare {
    T a, b;

    public:
    Compare(T a, T b) : a(std::move(a)), b(std::move(b)) {}

    ~Compare() = default;

    void max() {
        cout << "max=";
        (a > b) ? cout << a << " " : cout << b << " ";
    }

    void min() {
        cout << "min= ";
        (a < b) ? cout << a << " " : cout << b << " ";
    }
};

int main() {
    Compare<int> x(3, 7);
    Compare<double> y(45.78, 93.6);
    Compare<string> z("a", "A");
    x.max();
    x.min();
    cout << endl;
    y.max();
    y.min();
    cout << endl;
    z.max();
    z.min();
    cout << endl;
    return 0;
}