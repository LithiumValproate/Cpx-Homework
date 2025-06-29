# 实验八 流类库与输入输出

**班级：软件24-1  姓名：张三**

## 一、实验目的

+ 学习标准输入输出及格式控制
+ 对文件的应用方法（二进制文件和文本文件）
+ 掌握对格式化输出中操纵符的使用

## 二、实验内容

1. 定义一个结构体`Student`，成员为姓名（string类型）、学号（string类型）、分数（double类型）。**（20分）**
2. 从键盘输入3名学生的信息，并将其以文本形式写到名为`students.txt`的文件中，每行一个学生信息，字段之间用空格分隔即可。**（30分）**
3. 从该文件`students.txt`中读取所有学生信息；并按如下格式输出到屏幕，要求成绩右对齐、宽度为10且小数点后保留一位显示；姓名和学号均为左对齐、宽度为10。**（50分）**

**示例输出：**

```
name      id             score
Lihua     202401          89.5
Wanglei   202402          93.0
Zhangwu   202403          78.0
```

## 三、实验步骤及结果

（在此部分列出**解题思路、程序代码和运行结果截图等三项内容**。请注意保持格式规范统一，截图清晰，大小合适）

#### 代码撰写基本规范：

+ 保持代码结构清晰，注意缩进和对齐；
+ 标识符命名遵循“见其名，知其意思”原则，建议用英文词汇或其简写。
+ 括号等成对输入，不易遗漏；
+ 提高代码可读性。合理使用段注释和行注释

#### 参考示例：

1. 定义一个输入结构体，从外部文本中读取输入信息，在程序运行界面输出

```c++
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
```

运行结果截图：

![1](8-1.png)

## 四、实验小结

（包括问题和解决办法、心得体会。**必须要有具体错误截图和针对性讨论，不能仅有文字文字说明。**）**(20分）**

#### 实验小结参考示例：

**问题与解决办法**

无

**心得体会**：

+ 编写C程序要遵循语法规范，特别是符号输入不仅要注意大小写，而且要注意英文输入法下输入。

+ 应根据编译提示，定位和修改程序中的错误。

#### 重要提示，各位同学务必遵照此规范和格式，使用markdown格式编辑实验报告并在系统中提交。本课程不接收其它方式提交的实验报告。切记切记！