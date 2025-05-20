#include <array>
#include <iostream>
using namespace std;

class SoldRes {
    int num;
    int quantity;
    double price;

public:
    static int totalQty;
    static double totalVal;

    SoldRes(int n, int q, double p) : num(n), quantity(q), price(p) {
        totalQty += q;
        totalVal += p * q;
    }

    ~SoldRes() {}

    int get_num() const { return num; }

    int get_quantity() const { return quantity; }

    double get_price() const { return price; }

    double get_value() const { return price * quantity; }
};

void print(const SoldRes& s) {
    cout << s.get_num() << "\t" << s.get_quantity() << "\t" << s.get_price() << endl;
}

int SoldRes::totalQty = 0;

double SoldRes::totalVal = 0;

double averagePrice(double value, int qty) {
    return value / qty;
}

int main() {
    cout << "销售情况如下：" << endl;
    array<SoldRes, 3> sellers = {
        SoldRes(101, 5, 23.5),
        SoldRes(102, 12, 24.56),
        SoldRes(103, 100, 21.5)
    };
    for (int i = 0; i < sellers.size(); i++)
        print(sellers[i]);
    cout << "总销售额：" << SoldRes::totalVal << endl;
    cout << "销售平均价格：" << averagePrice(SoldRes::totalVal, SoldRes::totalQty) << endl;
    return 0;
}