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
    
    Student student;
    student.set_id(202401001);
    student.set_name("张三");
    student.set_sex(Sex::Male);
    student.set_birthdate(birthdate);
    student.set_admission_year(2024);
    student.set_major("计算机科学与技术");
    student.set_contact(contact);
    student.set_address(address);

    // 测试基本信息
    assert(student.get_id() == 202401001);
    assert(student.get_name() == "张三");
    assert(student.get_sex() == Sex::Male);
    assert(student.get_admission_year() == 2024);
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
    student.add_family_member(father);

    auto family = student.get_family_members();
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
    
    Stu_withScore student;
    student.set_id(202401002);
    student.set_name("李四");
    student.set_sex(Sex::Female);
    student.set_birthdate(birthdate);
    student.set_admission_year(2024);
    student.set_major("软件工程");
    student.set_contact(contact);
    student.set_address(address);

    // 测试继承的功能
    assert(student.get_id() == 202401002);
    assert(student.get_name() == "李四");
    assert(student.get_sex() == Sex::Female);
    
    // 测试成绩管理
    // 使用 courseName, score 和 gpa
    student.add_score("高等数学", Score(85.5, 3.5));
    student.add_score("大学物理", Score(92.0, 4.0));
    student.add_score("大学英语", Score(78.5, 3.0));

    auto scores = student.get_all_scores();
    assert(scores.size() == 3);
    
    // 测试查找成绩
    auto it = scores.find("大学物理");
    assert(it != scores.end());
    assert(it->second.score == 92.0);
    assert(it->second.gpa == 4.0);
    assert(scores.find("不存在的课程") == scores.end());

    // 测试平均分计算
    double avg_score = student.calculate_average();
    double expected_score_avg = (85.5 + 92.0 + 78.5) / 3.0;
    assert(abs(avg_score - expected_score_avg) < 0.001);
    std::cout << "计算得到的平均分: " << avg_score << std::endl;

    // 测试清空成绩
    student.set_scores({});
    assert(student.get_all_scores().empty());
    assert(student.calculate_average() == 0.0);

    std::cout << "Stu_withScore 类测试通过！" << std::endl;
    
    // 重新添加一些成绩用于显示
    student.add_score("C++程序设计", Score(88.0, 3.8));
    student.add_score("数据库原理", Score(91.5, 4.2));

    std::cout << student << std::endl;
}

// 测试Score结构体
void test_score() {
    std::cout << "\n=== 测试 Score 结构体 ===" << std::endl;
    
    // 测试默认构造函数
    Score score1;
    assert(score1.score == 0);
    assert(score1.gpa == 0);
    
    // 测试参数构造函数
    Score score2(95.5, 4.5);
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
    assert(empty_score_student.get_all_scores().empty());
    assert(empty_score_student.calculate_average() == 0.0);

    // 测试重复删除同一门课程
    Stu_withScore student;
    student.add_score("测试课程", Score(80, 3.0));
    
    // 直接使用 set_scores 清空
    student.set_scores({});
    assert(student.get_all_scores().empty());

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