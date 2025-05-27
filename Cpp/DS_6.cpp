#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>
#include <string>
#include <utility>
#include <vector>

using namespace std;

// 查找结果结构体：存储索引和步数
struct Answer {
    int index;
    int steps;
};

// 数据分布类型
enum class e_DistType { RANDOM, SORTED, REVERSED };

// 生成 0–9999 之间的随机整数序列
vector<int> gen_data(size_t n, e_DistType dt) {
    vector<int> v(n);
    mt19937_64 rng(static_cast<unsigned long>(
                       chrono::high_resolution_clock::now().time_since_epoch().count()
                   ));
    uniform_int_distribution<int> dist(0, 9999);
    for (size_t i = 0; i < n; ++i) {
        v[i] = dist(rng);
    }
    if (dt == e_DistType::SORTED) {
        sort(v.begin(), v.end());
    } else if (dt == e_DistType::REVERSED) {
        sort(v.begin(), v.end(), greater<>());
    }
    return v;
}

// 抽象基类：定义统一的查找接口
class List {
protected:
    size_t size = 0;

public:
    virtual ~List() = default;

    virtual Answer sequential_search(const int& key) = 0;

    virtual Answer binary_search(const int& key) = 0;
};

// 顺序表 (ArrayList) 实现
class ArrayList final : public List {
private:
    vector<int> data;                       // 原始数据
    vector<pair<int, unsigned>> sortedData; // (值, 原始索引) 用于折半查找

public:
    // 构造并复制数据，同时建立并排序 sortedData
    explicit ArrayList(const vector<int>& data_)
        : data(data_) {
        this->size = data.size();
        sortedData.reserve(this->size);
        for (unsigned i = 0; i < this->size; ++i) {
            sortedData.emplace_back(data[i], i);
        }
        sort(sortedData.begin(), sortedData.end(),
             [] (auto& a, auto& b) { return a.first < b.first; });
    }

    ~ArrayList() override = default;

    Answer sequential_search(const int& key) override {
        int steps = 0;
        for (size_t i = 0; i < this->size; ++i) {
            ++steps;
            if (data[i] == key) {
                return {static_cast<int>(i), steps};
            }
        }
        return {-1, steps};
    }

    Answer binary_search(const int& key) override {
        int steps = 0;
        int left  = 0;
        int right = static_cast<int>(this->size) - 1;
        while (left <= right) {
            ++steps;
            int mid = left + (right - left) / 2;
            if (sortedData[mid].first == key) {
                return {static_cast<int>(sortedData[mid].second), steps};
            }
            if (sortedData[mid].first < key) {
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }
        return {-1, steps};
    }
};

// 双向链表 (LinkedList) 实现
class LinkedList final : public List {
private:
    struct Node {
        int data;
        Node* prev;
        Node* next;
        explicit Node(int v) : data(v), prev(nullptr), next(nullptr) {}
    };

    Node* head = nullptr;
    Node* tail = nullptr;

public:
    LinkedList() { this->size = 0; }

    ~LinkedList() override {
        Node* curr = head;
        while (curr) {
            Node* next_ = curr->next;
            delete curr;
            curr = next_;
        }
    }

    // 末尾插入
    void push_back_(int v) {
        Node* newNode = new Node(v);
        if (!head) {
            head = tail = newNode;
        } else {
            tail->next    = newNode;
            newNode->prev = tail;
            tail          = newNode;
        }
        ++this->size;
    }

    Answer sequential_search(const int& key) override {
        int steps  = 0;
        Node* curr = head;
        int idx    = 0;
        while (curr) {
            ++steps;
            if (curr->data == key) {
                return {idx, steps};
            }
            curr = curr->next;
            ++idx;
        }
        return {-1, steps};
    }

    // 链表上不支持二分，直接调用顺序查找
    Answer binary_search(const int& key) override {
        return sequential_search(key);
    }
};

// “计时 + 调用查找”模板函数
template<typename Container> void time_search(Container& container,
                                              Answer (Container::*search_fn)(const int&),
                                              const string& name,
                                              const int& key) {
    auto start        = chrono::high_resolution_clock::now();
    Answer ans        = (container.*search_fn)(key);
    auto end          = chrono::high_resolution_clock::now();
    long long elapsed = chrono::duration_cast<chrono::microseconds>(end - start).count();

    cout << name
            << " -> index: " << ans.index
            << ", steps: " << ans.steps
            << ", time: " << elapsed << " µs\n";
}

int main() {
    int loop = 1;
    do {
        // 1. 输入元素个数
        size_t n;
        cout << "Enter number of elements: ";
        cin >> n;

        // 2. 选择数据分布
        int choice;
        cout << "Select data distribution:\n"
                << "  1. Random\n"
                << "  2. Sorted\n"
                << "  3. Reversed\n"
                << "Enter choice (1-3): ";
        cin >> choice;

        // 3. 输入要查找的键值
        int key;
        cout << "Enter search key (integer): ";
        cin >> key;

        // 4. 映射到枚举类型
        e_DistType dt = e_DistType::RANDOM;
        if (choice == 2) dt = e_DistType::SORTED;
        else if (choice == 3) dt = e_DistType::REVERSED;

        // 5. 生成测试数据
        vector<int> data = gen_data(n, dt);

        // 6. 构建顺序表和链表
        ArrayList arrL(data);
        LinkedList linkL;
        for (int v : data) {
            linkL.push_back_(v);
        }

        // 7. 在顺序表中测试
        cout << "\n--- ArrayList Searches ---\n";
        time_search(arrL, &ArrayList::sequential_search, "Sequential Search", key);
        time_search(arrL, &ArrayList::binary_search, "Binary Search", key);

        // 8. 在链表中测试
        cout << "\n--- LinkedList Searches ---\n";
        time_search(linkL, &LinkedList::sequential_search, "Sequential Search", key);
        time_search(linkL, &LinkedList::binary_search, "Binary Search", key);

        // 9. 是否结束测试
        cout << endl << "Test again or input 0 to exit." << endl;
        cin >> loop;
    } while (loop);
    return 0;
}
