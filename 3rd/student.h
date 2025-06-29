#pragma once

#include <algorithm>
#include <ctime>
#include <ostream>
#include <string>
#include <vector>
#include "json.hpp"

struct Date {
    int year;
    int month;
    int day;
    Date() : year(0), month(0), day(0) {}
    Date(int y, int m, int d) : year(y), month(m), day(d) {}

    nlohmann::json to_json() const {
        return nlohmann::json{
            {"year", year},
            {"month", month},
            {"day", day}
        };
    }

    void from_json(const nlohmann::json& j) {
        year = j.at("year").get<int>();
        month = j.at("month").get<int>();
        day = j.at("day").get<int>();
    }
};

struct Address {
    std::string city;
    std::string province;
    Address() : city(""), province("") {}
    Address(const std::string& c, const std::string& p) : city(c), province(p) {}

    nlohmann::json to_json() const {
        return nlohmann::json{
            {"city", city},
            {"province", province}
        };
    }

    void from_json(const nlohmann::json& j) {
        city = j.at("city").get<std::string>();
        province = j.at("province").get<std::string>();
    }
};

struct Contact {
    std::string phone;
    std::string email;
    Contact() : phone(""), email("") {}
    Contact(const std::string& p, const std::string& e) : phone(p), email(e) {}

    nlohmann::json to_json() const {
        return nlohmann::json{
            {"phone", phone},
            {"email", email}
        };
    }

    void from_json(const nlohmann::json& j) {
        phone = j.at("phone").get<std::string>();
        email = j.at("email").get<std::string>();
    }
};

struct FamilyMember {
    std::string name;
    std::string relationship;
    Contact contactInfo;
    FamilyMember() : name(""), relationship(""), contactInfo() {}
    FamilyMember(const std::string& n, const std::string& r, const Contact& c)
        : name(n), relationship(r), contactInfo(c) {}

    nlohmann::json to_json() const {
        return nlohmann::json{
            {"name", name},
            {"relationship", relationship},
            {"contactInfo", contactInfo.to_json()}
        };
    }

    void from_json(const nlohmann::json& j) {
        name = j.at("name").get<std::string>();
        relationship = j.at("relationship").get<std::string>();
        contactInfo.from_json(j.at("contactInfo"));
    }
};

enum struct Status { Active, Leave, Graduated };

class Student {
private:
    long id;
    std::string name;
    std::string sex;
    Date birthdate;
    int admissionYear;
    std::string major;
    std::vector<std::string> courses;
    Contact contactInfo;
    Address address;
    std::vector<FamilyMember> familyMembers;
    Status status{Status::Active};

public:
    Student()
      : id(0), name(""), sex(""), birthdate(), admissionYear(0),
        major(""), contactInfo(), address(), status(Status::Active) {}

    Student(long id_,
            const std::string& n,
            const std::string& s,
            const Date& d,
            int y,
            const std::string& maj,
            const Contact& contact,
            const Address& addr,
            Status st = Status::Active)
      : id(id_), name(n), sex(s), birthdate(d),
        admissionYear(y), major(maj),
        contactInfo(contact), address(addr), status(st) {}

    ~Student() = default;

    // 学号
    long get_id() const { return id; }
    void set_id(long newId) { id = newId; }

    // 姓名
    auto get_name() const -> const std::string& { return name; }
    void set_name(const std::string& newName) { name = newName; }

    // 性别
    auto get_sex() const -> const std::string& { return sex; }
    void set_sex(const std::string& newSex) { sex = newSex; }

    // 年龄
    int calculate_age() const {
        std::time_t t = std::time(nullptr);
        std::tm* now = std::localtime(&t);
        int age = (now->tm_year + 1900) - birthdate.year;
        if ((now->tm_mon + 1) < birthdate.month ||
            ((now->tm_mon + 1) == birthdate.month && now->tm_mday < birthdate.day)) {
            --age;
        }
        return age;
    }
    int get_age() const { return calculate_age(); }

    // 生日
    auto get_birthdate() const -> const Date& { return birthdate; }
    void set_birthdate(const Date& newDate) { birthdate = newDate; }

    // 入学年份
    int get_admissionYear() const { return admissionYear; }
    void set_admissionYear(int newYear) { admissionYear = newYear; }

    // 专业
    auto get_major() const -> const std::string& { return major; }
    void set_major(const std::string& newMajor) { major = newMajor; }

    // 课程
    auto get_courses() const -> const std::vector<std::string>& { return courses; }
    void add_course(const std::string& newCourse) {
        courses.push_back(newCourse);
    }
    void del_course(const std::string& course) {
        courses.erase(std::remove(courses.begin(), courses.end(), course), courses.end());
    }

    // 联系方式
    Contact get_contact() const { return contactInfo; }
    void set_contact(const Contact& newContact) { contactInfo = newContact; }

    // 地址
    Address get_address() const { return address; }
    void set_address(const Address& newAddress) { address = newAddress; }

    // 家庭成员
    auto get_familyMembers() const -> const std::vector<FamilyMember>& { return familyMembers; }
    void add_familyMember(const FamilyMember& newFamilyMember) {
        familyMembers.push_back(newFamilyMember);
    }
    void del_familyMember(const FamilyMember& familyMember) {
        familyMembers.erase(
            std::remove_if(familyMembers.begin(), familyMembers.end(),
                [&familyMember](const FamilyMember& fm) {
                    return fm.name == familyMember.name
                        && fm.relationship == familyMember.relationship;
                }),
            familyMembers.end());
    }

    Status get_status() const { return status; }
    void set_status(Status s) { status = s; }

    nlohmann::json to_json() const {
        nlohmann::json j = {
                {"id", id},
                {"name", name},
                {"birthdate", {{"year", birthdate.year}, {"month", birthdate.month}, {"day", birthdate.day}}},
                {"admissionYear", admissionYear},
                {"major", major},
                {"courses", courses},
                {"contact", {{"phone", contactInfo.phone}, {"email", contactInfo.email}}},
                {"address", {{"province", address.province}, {"city", address.city}}},
                {"status", status == Status::Active ? "Active" : status == Status::Leave ? "Leave" : "Graduated"},
                {"familyMembers", nlohmann::json::array()}
                };

        for (const auto& member : familyMembers) {
            j["familyMembers"].push_back(member.to_json());
        }

        return j;
    }
    void from_json(const nlohmann::json& j) {
        id = j.at("id").get<long>();
        name = j.at("name").get<std::string>();
        birthdate.year  = j.at("birthdate").at("year");
        birthdate.month = j.at("birthdate").at("month");
        birthdate.day   = j.at("birthdate").at("day");
        admissionYear = j.at("admissionYear");
        major = j.at("major").get<std::string>();
        courses = j.at("courses").get<std::vector<std::string>>();
        contactInfo.phone = j.at("contact").at("phone");
        contactInfo.email = j.at("contact").at("email");
        address.province  = j.at("address").at("province");
        address.city      = j.at("address").at("city");
        std::string st    = j.at("status").get<std::string>();
        status = (st == "Leave" ? Status::Leave :
                 (st == "Graduated" ? Status::Graduated : Status::Active));

        familyMembers.clear();
        if (j.contains("familyMembers") && j["familyMembers"].is_array()) {
            for (const auto& memberJson : j["familyMembers"]) {
                FamilyMember member;
                member.from_json(memberJson);
                familyMembers.push_back(member);
            }
        }
    }

    friend std::ostream& operator<<(std::ostream& os, const Student& s) {
        os << "学号: " << s.id << "\n"
           << "姓名: " << s.name << "\n"
           << "年龄: " << s.get_age() << "\n"
           << "生日: " << s.birthdate.year << "-" << s.birthdate.month << "-" << s.birthdate.day << "\n"
           << "入学年份: " << s.admissionYear << "\n"
           << "专业: " << s.major << "\n"
           << "电话: " << s.contactInfo.phone << ", 邮箱: " << s.contactInfo.email << "\n"
           << "地址: " << s.address.province << "省，" << s.address.city << "市\n"
           << "在读状态: "
           << (s.status == Status::Active ? "在读" :
               s.status == Status::Leave  ? "休学" : "毕业") << "\n";
        return os;
    }
};