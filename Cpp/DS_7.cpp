#include <functional>
#include <iostream>
#include <string>
#include <utility>
#include <vector>
using namespace std;

//定义Student类，由ID，姓名，分数构成
class Student {
    int id;
    string name;
    double score;

public:
    //构造Student对象
    Student(const int i, string  n, const double s) : id(i), name(std::move(n)), score(s) {}

    //复制构造Student对象
    Student(const Student&) = default;

    ~Student() = default;

    //重载Student赋值运算
    Student& operator=(const Student&) = default;

    int get_id() const { return id; }

    const string& get_name() const { return name; }

    double get_score() const { return score; }

    /* 静态函数，用于根据输入的排序模式返回相应的比较器
     * 返回值是一个std::function，内部封装了对应排序规则的lambda表达式。
     * 通过输入mode（1: 按 id，2: 按 name，其它: 按 score）选择排序方式。
     * 返回的 lambda 用于对 Student 对象按不同属性进行比较。
     */
    static auto get_comparator(const int mode) -> function<bool(const Student&, const Student&)> {
        switch (mode) {
            case 1:
                return [] (const Student& a, const Student& b) {
                    return a.id < b.id;
                };
            case 2:
                return [] (const Student& a, const Student& b) {
                    return a.name < b.name;
                };
            default:
                return [] (const Student& a, const Student& b) {
                    return a.score < b.score;
                };
        }
    }
};

//定义成绩单的基类，包含每名学生数据的数组，为排序类提供数据支持
class Report {
protected:
    int n = 0;
    vector<Student> students;

public:
    Report(Student* s, const int n) : n(n) {
        students.assign(s, s + n);
    }

    ~Report() = default;
};

//成绩处理类：继承自 Report，封装多种排序算法以对学生数据进行操作
class Solution final : public Report {
    vector<Student> resStu; //排序结果

    void reset() { resStu = students; } //重置结果，以便进行多次排序测试

public:
    Solution(Student* arr, const int n) : Report(arr, n) {
        reset();
    }

    /* 以下排序函数均接受一个参数 cmp
     * cmp 是一个函数对象（类型为 std::function<bool(Student&, Student&)>
     * 由 Student::get_comparator(mode) 提供，封装了具体的排序规则
     * cmp(a, b) 表示：若 a 应排在 b 之前，则返回 true
     * 通过 cmp 的不同实现，排序函数可以支持按 id、name 或 score 排序
     */

    //直接插入排序
    void straight_insertion_sort(const function<bool(const Student&, const Student&)>& cmp) {
        reset();
        for (size_t i = 1; i < resStu.size(); i++) {
            Student key = resStu[i];
            int j       = static_cast<int>(i) - 1;
            while (j >= 0 && cmp(key, resStu[j])) {
                resStu[j + 1] = resStu[j];
                j--;
            }
            resStu[j + 1] = key;
        }
    }

    //二分插入排序
    void binary_insertion_sort(const function<bool(const Student&, const Student&)>& cmp) {
        reset();
        for (size_t i = 1; i < resStu.size(); i++) {
            Student key = resStu[i];
            int left    = 0, right = static_cast<int>(i) - 1;
            while (left <= right) {
                int mid = left + (right - left) / 2;
                if (cmp(key, resStu[mid]))
                    right = mid - 1;
                else
                    left = mid + 1;
            }
            for (int j        = static_cast<int>(i) - 1; j >= left; j--)
                resStu[j + 1] = resStu[j];
            resStu[left] = key;
        }
    }

    //冒泡排序
    void bubble_sort(const function<bool(const Student&, const Student&)>& cmp) {
        reset();
        for (size_t i = 0; i + 1 < resStu.size(); i++)
            for (size_t j = 0; j + 1 < resStu.size() - i; j++)
                if (cmp(resStu[j + 1], resStu[j]))
                    swap(resStu[j], resStu[j + 1]);
    }

    //选择排序
    void selection_sort(const function<bool(const Student&, const Student&)>& cmp) {
        reset();
        for (size_t i = 0; i + 1 < resStu.size(); i++) {
            size_t min_idx = i;
            for (size_t j = i + 1; j < resStu.size(); j++)
                if (cmp(resStu[j], resStu[min_idx]))
                    min_idx = j;
            swap(resStu[i], resStu[min_idx]);
        }
    }

    //快速排序，使用lambda进行递推
    void quick_sort(const function<bool(const Student&, const Student&)>& cmp) {
        reset();
        function<void(int, int)> quick_sort_ = [&] (int l, int r) -> void {
            if (l >= r) return;
            Student pivot = resStu[l + (r - l) / 2];
            int i         = l, j = r;
            while (i <= j) {
                while (cmp(resStu[i], pivot)) ++i;
                while (cmp(pivot, resStu[j])) --j;
                if (i <= j) {
                    swap(resStu[i], resStu[j]);
                    ++i;
                    --j;
                }
            }
            if (l < j) quick_sort_(l, j);
            if (i < r) quick_sort_(i, r);
        };
        quick_sort_(0, static_cast<int>(resStu.size()) - 1);
    }

    vector<Student> get_sorted_students() const {
        return resStu;
    }

    void print() const {
        for (const auto& i : resStu)
            cout << i.get_id() << " "
                    << i.get_name() << " "
                    << i.get_score() << endl;
    }
};

int main() {
    Student arr[] = {
            Student(1003, "Alice", 92.5),
            Student(1001, "Charlie", 86.0),
            Student(1002, "Bob", 78.3),
            Student(1004, "David", 89.7),
            Student(1005, "Eve", 95.2)
            };
    const Solution solution(arr, 5);
    int c = 0;
    do {
        solution.print();
        cout << "Please confirm the infos.\n"
                << "Press 1 to continue...";
        cin >> c;
    } while (c != 1);

    constexpr int n = sizeof(arr) / sizeof(Student);

    for (int mode = 1; mode <= 3; mode++) {
        const string sortNames[] = {
                "Straight Insertion Sort",
                "Binary Insertion Sort",
                "Bubble Sort",
                "Selection Sort",
                "Quick Sort"
                };
        auto cmp = Student::get_comparator(mode);
        cout << "\n=== Sorting by " << (mode == 1 ? "ID" : mode == 2 ? "Name" : "Score") << " ===\n"; {
            Solution s(arr, n);
            cout << "--- " << sortNames[0] << " ---\n";
            s.straight_insertion_sort(cmp);
            s.print();
        } {
            Solution s(arr, n);
            cout << "--- " << sortNames[1] << " ---\n";
            s.binary_insertion_sort(cmp);
            s.print();
        } {
            Solution s(arr, n);
            cout << "--- " << sortNames[2] << " ---\n";
            s.bubble_sort(cmp);
            s.print();
        } {
            Solution s(arr, n);
            cout << "--- " << sortNames[3] << " ---\n";
            s.selection_sort(cmp);
            s.print();
        } {
            Solution s(arr, n);
            cout << "--- " << sortNames[4] << " ---\n";
            s.quick_sort(cmp);
            s.print();
        }
    }
    return 0;
}
