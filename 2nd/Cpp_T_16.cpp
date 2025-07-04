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

    ~SoldRes() = default;

    int get_num() const { return num; }

    int get_quantity() const { return quantity; }

    double get_price() const { return price; }
};

void print(const SoldRes& s) {
    cout << s.get_num() << "\t" << s.get_quantity() << "\t" << s.get_price() << endl;
}

int SoldRes::totalQty = 0;

double SoldRes::totalVal = 0;

int main() {
    cout << "销售情况如下：" << endl;
    array<SoldRes, 3> sellers = {
            SoldRes(101, 5, 23.5),
            SoldRes(102, 12, 24.56),
            SoldRes(103, 100, 21.5)
            };
    for (const auto& seller : sellers)
        print(seller);
    cout << "总销售额：" << SoldRes::totalVal << endl;
    cout << "销售平均价格：" << SoldRes::totalVal / SoldRes::totalQty << endl;
    return 0;
}
