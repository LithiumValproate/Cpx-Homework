# 实验七 模板与群体数据

__班级：软件24-18  姓名：杨鎏__

## 一、实验目的

* 掌握函数模版与类模版的定义及使用
* 掌握群体数据的访问方式

## 二、实验内容

#### 1. 设计线性表抽象基类模板与派生类 (50分)

设计一个线性表抽象基类模板`LinearList<T>`，包含以下函数：

* `virtual T& getElement(int position) = 0 `: 获取指定位置的数据元素
* `virtual void insert(int position, const T& elem) = 0`：插入元素
* `virtual ~LinearList() {} ` ：虚析构函数

从该抽象基类派生两个子类模板：

* `ArrayList<T>`: 基于数组实现的线性表
* `LinkedList<T>`: 基于链表实现的线性表

两个派生类都需要实现基类的纯虚函数，并根据各自特点 __仅添加必要的辅助完成查找操作的成员变量、成员函数和其他代码__。

#### 2. 设计通用查找函数模板 (20分)

`T find(LinearList<T>& list, int position)`：用于查找线性表中指定位置的数据元素并返回（可借助`getElement`函数完成操作）。

#### 3. 主函数测试 (30分)

在`main`函数中完成以下测试：

1. 整数类型测试：
   * 创建整数类型的数组和链表这两种线性表
   * 向两种线性表中分别添加多个整数元素
   * 测试`find`函数，查找两种线性表里第3个位置的数据元素并打印
2. 浮点数类型测试：
   * 创建float类型的数组线性表和链表线性表
   * 向两种线性表中分别添加多个小数
   * 测试`find`函数，查找两种线性表里第3个位置的数据元素并打印

## 三、实验步骤及结果

（在此部分列出 __解题思路、程序代码和运行结果截图等三项内容__。请注意保持格式规范统一，截图清晰，大小合适）

#### 代码撰写基本规范：

* 保持代码结构清晰，注意缩进和对齐；
* 标识符命名遵循“见其名，知其意思”原则，建议用英文词汇或其简写。
* 括号等成对输入，不易遗漏；
* 提高代码可读性。合理使用段注释和行注释

#### 参考示例：

0. 编写程序，在屏幕上输出“Hello World"

```cpp
/*
  功能：在屏幕上输出“Hello World"
  作者：张三
  版本：1.0
*/
#include <iostream>
using namespace std;
int main(){
    cout << "Hello World" << endl;
    return 0;
}
```

运行结果截图：

<img src="https://gitee.com/yannyyy/object-oriented-programming/raw/master/imgs/helloworld.png" alt="Hello World示例" style="zoom:150%;" />

1. 设计线性表抽象基类模板与派生类，找函数模板，并整数、浮点数类型测试：

```cpp
#include <iostream>
#include <vector>
using namespace std;

inline bool is_valid(int i, int n) {
    return 0 <= i && i < n;
}

#define ASSERT_VALID(cond) do { \
if (!(cond)) { \
cerr << "Invalid position" << endl; \
exit(1); \
} \
} while (0)

template<typename T> class LinearList {
public:
    virtual ~LinearList() = default;

    virtual T& get_element(int position) = 0;

    virtual void insert(int position, const T& elem) = 0;
};

template<typename T> class ArrayList : public LinearList<T> {
    vector<T> data;

public:
    T& get_element(int position) override {
        ASSERT_VALID(is_valid(position, data.size()));
        return data[position];
    }

    void insert(int position, const T& elem) override {
        ASSERT_VALID(is_valid(position, data.size() + 1));
        data.insert(data.begin() + position, elem);
    }
};

template<typename T> class LinkedList : public LinearList<T> {
    struct Node {
        T value;
        Node* next;

        Node(const T& value, Node* next = nullptr)
            : value(value), next(next) {}
    };

    Node* head;
    unsigned size;

public:
    LinkedList() : head(nullptr), size(0) {}

    ~LinkedList() override {
        while (head) {
            Node* temp = head;
            head       = head->next;
            delete temp;
        }
    }

    T& get_element(int position) override {
        ASSERT_VALID(is_valid(position, size));
        Node* curr = head;
        for (int i = 0; i < position; i++)
            curr   = curr->next;
        return curr->value;
    }

    void insert(int position, const T& elem) override {
        ASSERT_VALID(is_valid(position, size + 1));
        Node* newNode = new Node(elem);
        if (position == 0) {
            newNode->next = head;
            head          = newNode;
        } else {
            Node* previousNode = head;
            for (int i       = 0; i < position - 1; i++)
                previousNode = previousNode->next;
            newNode->next      = previousNode->next;
            previousNode->next = newNode;
        }
        ++size;
    }
};

template<typename T> T find(LinearList<T>& list, int position) {
    return list.get_element(position);
}

int main() {
    ArrayList<int> intArray;
    LinkedList<int> intLinked;
    intArray.insert(0, 10);
    intArray.insert(1, 20);
    intArray.insert(2, 30);
    intArray.insert(3, 40);
    intLinked.insert(0, 10);
    intLinked.insert(1, 20);
    intLinked.insert(2, 30);
    intLinked.insert(3, 40);
    cout << "ArrayList<int> element at 3rd position: " << find(intArray, 2) << endl;
    cout << "LinkedList<int> element at 3rd position: " << find(intLinked, 2) << endl;

    ArrayList<float> floatArray;
    LinkedList<float> floatLinked;
    floatArray.insert(0, 1.1f);
    floatArray.insert(1, 2.2f);
    floatArray.insert(2, 3.3f);
    floatArray.insert(3, 4.4f);
    floatLinked.insert(0, 1.1f);
    floatLinked.insert(1, 2.2f);
    floatLinked.insert(2, 3.3f);
    floatLinked.insert(3, 4.4f);
    cout << "ArrayList<float> element at 3rd position: " << find(floatArray, 2) << endl;
    cout << "LinkedList<float> element at 3rd position: " << find(floatLinked, 2) << endl;

    return 0;
}
```

运行结果截图：

![1](7.png)

## 四、实验小结

（包括问题和解决办法、心得体会。 __必须要有具体错误截图和针对性讨论，不能仅有文字文字说明。__）__（20分）__

#### 实验小结参考示例：

__问题与解决办法：__ 无

__解决办法：__ 查找资料后，发现中文状态输入双引号，系统只允许英文状态下双引号字符。改正后，错误消失,程序运行正常。

__心得体会：代码健壮性很重要__

#### 重要提示，各位同学务必遵照此规范和格式，使用markdown格式编辑实验报告并在系统中提交。本课程不接收其它方式提交的实验报告。切记切记！