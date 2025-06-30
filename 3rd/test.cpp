#include <iostream>
#include <cassert>
#include <string>
#include <vector>
#include "student.h"
#include "stu_with_score.h"

// 测试基础Student类
void test_student() {
    std::cout << "=== 测试 Student 类 ===" << std::endl;
    
    // 创建学生对象
    Date birthdate(2000, 3, 15);
    Contact contact("13800138000", "zhang@example.com");
    Address address("北京", "北京");
    
    Student student(202401001, "张三", Sex::Male, birthdate, 2024, 
                   "计算机科学与技术", contact, address);
    
    // 测试基本信息
    assert(student.get_id() == 202401001);
    assert(student.get_name() == "张三");
    assert(student.get_sex() == Sex::Male);
    assert(student.get_admissionYear() == 2024);
    assert(student.get_major() == "计算机科学与技术");
    
    // 测试年龄计算
    int age = student.get_age();
    std::cout << "学生年龄: " << age << std::endl;
    
    // 测试课程管理
    student.add_course("数据结构");
    student.add_course("算法设计");
    student.add_course("操作系统");
    
    auto courses = student.get_courses();
    assert(courses.size() == 3);
    assert(courses[0] == "数据结构");
    
    student.del_course("算法设计");
    courses = student.get_courses();
    assert(courses.size() == 2);
    
    // 测试家庭成员
    Contact parent_contact("13900139000", "parent@example.com");
    FamilyMember father("张大明", "父亲", parent_contact);
    student.add_familyMember(father);
    
    auto family = student.get_familyMembers();
    assert(family.size() == 1);
    assert(family[0].name == "张大明");
    
    // 测试状态修改
    student.set_status(Status::Active);
    assert(student.get_status() == Status::Active);
    
    std::cout << "Student 类测试通过！" << std::endl;
    std::cout << student << std::endl;
}

// 测试带成绩的学生类
void test_stu_with_score() {
    std::cout << "\n=== 测试 Stu_withScore 类 ===" << std::endl;
    
    // 创建带成绩的学生对象
    Date birthdate(2001, 8, 20);
    Contact contact("13700137000", "li@example.com");
    Address address("上海", "上海");
    
    Stu_withScore student(202401002, "李四", Sex::Female, birthdate, 2024,
                         "软件工程", contact, address);
    
    // 测试继承的功能
    assert(student.get_id() == 202401002);
    assert(student.get_name() == "李四");
    assert(student.get_sex() == Sex::Female);
    
    // 测试成绩管理
    Score math("高等数学", 85.5, 3.5);
    Score physics("大学物理", 92.0, 4.0);
    Score english("大学英语", 78.5, 3.0);
    
    student.add_score(math);
    student.add_score(physics);
    student.add_score(english);
    
    auto scores = student.get_scores();
    assert(scores.size() == 3);
    
    // 测试查找成绩
    Score* found = student.find_score("大学物理");
    assert(found != nullptr);
    assert(found->score == 92.0);
    assert(found->gpa == 4.0);
    
    Score* not_found = student.find_score("不存在的课程");
    assert(not_found == nullptr);
    
    // 测试GPA计算
    double gpa = student.calculate_gpa();
    double expected_gpa = (3.5 + 4.0 + 3.0) / 3.0;
    assert(abs(gpa - expected_gpa) < 0.001);
    std::cout << "计算得到的GPA: " << gpa << std::endl;
    
    // 测试删除成绩
    bool removed = student.remove_score("大学英语");
    assert(removed == true);
    
    scores = student.get_scores();
    assert(scores.size() == 2);
    
    bool not_removed = student.remove_score("不存在的课程");
    assert(not_removed == false);
    
    // 测试清空成绩
    student.clear_scores();
    scores = student.get_scores();
    assert(scores.empty());
    
    // 空成绩列表的GPA应该为0
    assert(student.calculate_gpa() == 0.0);
    
    std::cout << "Stu_withScore 类测试通过！" << std::endl;
    
    // 重新添加一些成绩用于显示
    student.add_score(Score("C++程序设计", 88.0, 3.8));
    student.add_score(Score("数据库原理", 91.5, 4.2));
    
    std::cout << student << std::endl;
}

// 测试Score结构体
void test_score() {
    std::cout << "\n=== 测试 Score 结构体 ===" << std::endl;
    
    // 测试默认构造函数
    Score score1;
    assert(score1.courseName == "");
    assert(score1.score == 0);
    assert(score1.gpa == 0);
    
    // 测试参数构造函数
    Score score2("线性代数", 95.5, 4.5);
    assert(score2.courseName == "线性代数");
    assert(score2.score == 95.5);
    assert(score2.gpa == 4.5);
    
    std::cout << "Score 结构体测试通过！" << std::endl;
}

// 测试边界情况和异常情况
void test_edge_cases() {
    std::cout << "\n=== 测试边界情况 ===" << std::endl;
    
    // 测试空学生
    Student empty_student;
    assert(empty_student.get_id() == 0);
    assert(empty_student.get_name() == "");
    
    // 测试空成绩学生
    Stu_withScore empty_score_student;
    assert(empty_score_student.get_scores().empty());
    assert(empty_score_student.calculate_gpa() == 0.0);
    
    // 测试重复删除同一门课程
    Stu_withScore student;
    student.add_score(Score("测试课程", 80, 3.0));
    
    bool first_remove = student.remove_score("测试课程");
    assert(first_remove == true);
    
    bool second_remove = student.remove_score("测试课程");
    assert(second_remove == false);
    
    std::cout << "边界情况测试通过！" << std::endl;
}

int main() {
    try {
        test_score();
        test_student();
        test_stu_with_score();
        test_edge_cases();
        
        std::cout << "\n🎉 所有测试通过！" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "测试失败: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}