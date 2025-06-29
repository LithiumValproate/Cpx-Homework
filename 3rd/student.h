#pragma once

#include <algorithm>
#include <ctime>
#include <ostream>
#include <string>
#include <vector>
#include "json.hpp"

using nlohmann::json;

// -- data types --

struct Date {
    int year{};
    int month{};
    int day{};

    Date() = default;
    Date(int y, int m, int d) : year(y), month(m), day(d) {}
};

struct Address {
    std::string city;
    std::string province;

    Address() = default;
    Address(const std::string& c, const std::string& p)
        : city(c), province(p) {}
};

struct Contact {
    std::string phone;
    std::string email;

    Contact() = default;
    Contact(const std::string& p, const std::string& e)
        : phone(p), email(e) {}
};

struct FamilyMember {
    std::string name;
    std::string relationship;
    Contact contactInfo;

    FamilyMember() = default;
    FamilyMember(const std::string& n,
                 const std::string& r,
                 const Contact& c)
        : name(n), relationship(r), contactInfo(c) {}
};

enum class Status { Active = 0, Leave = 1, Graduated = 2 };

class Student {
private:
    long id{};
    std::string name;
    std::string sex;
    Date birthdate;
    int admissionYear{};
    std::string major;
    std::vector<std::string> courses;
    Contact contactInfo;
    Address address;
    std::vector<FamilyMember> familyMembers;
    Status status{Status::Active};

public:
    Student() = default;
    Student(long id,
            const std::string& n,
            const std::string& s,
            const Date& d,
            int y,
            const std::string& maj,
            const Contact& contact,
            const Address& addr,
            Status st = Status::Active)
        : id{id}, name{n}, sex{s}, birthdate{d},
          admissionYear{y}, major{maj},
          contactInfo{contact}, address{addr}, status{st} {}

    // copy/move
    Student(const Student&) = default;
    Student(Student&&) noexcept = default;
    Student& operator=(const Student&) = default;
    Student& operator=(Student&&) noexcept = default;

    // getters/setters
    auto get_id() const -> long { return id; }
    void set_id(long v) { id = v; }

    auto get_name() const -> const std::string& { return name; }
    void set_name(const std::string& v) { name = v; }

    auto get_sex() const -> const std::string& { return sex; }
    void set_sex(const std::string& v) { sex = v; }

    auto get_birthdate() const -> const Date& { return birthdate; }
    void set_birthdate(const Date& v) { birthdate = v; }

    auto get_admissionYear() const -> int { return admissionYear; }
    void set_admissionYear(int v) { admissionYear = v; }

    auto get_major() const -> const std::string& { return major; }
    void set_major(const std::string& v) { major = v; }

    auto get_courses() const -> const std::vector<std::string>& { return courses; }
    void add_course(const std::string& c) { courses.push_back(c); }
    void del_course(const std::string& c) {
        courses.erase(
                      std::remove(courses.begin(), courses.end(), c),
                      courses.end());
    }

    auto get_contact() const -> Contact { return contactInfo; }
    void set_contact(const Contact& v) { contactInfo = v; }

    auto get_address() const -> Address { return address; }
    void set_address(const Address& v) { address = v; }

    auto get_familyMembers() const -> const std::vector<FamilyMember>& {
        return familyMembers;
    }
    void set_familyMembers(const std::vector<FamilyMember>& v) {
        familyMembers = v;
    }
    void add_familyMember(const FamilyMember& fm) {
        familyMembers.push_back(fm);
    }
    void del_familyMember(const FamilyMember& fm) {
        familyMembers.erase(
                            std::remove_if(
                                           familyMembers.begin(),
                                           familyMembers.end(),
                                           [&] (auto const& m) {
                                               return m.name == fm.name
                                                      && m.relationship == fm.relationship;
                                           }),
                            familyMembers.end());
    }

    auto get_status() const -> Status { return status; }
    void set_status(Status s) { status = s; }

    // age calculation
    int calculate_age() const {
        std::time_t t = std::time(nullptr);
        std::tm* now  = std::localtime(&t);
        int age       = now->tm_year + 1900 - birthdate.year;
        if ((now->tm_mon + 1) < birthdate.month ||
            ((now->tm_mon + 1) == birthdate.month &&
             now->tm_mday < birthdate.day)) {
            --age;
        }
        return age;
    }
    int get_age() const { return calculate_age(); }

    // streaming
    friend std::ostream& operator<<(std::ostream& os,
                                    const Student& s) {
        os << "学号: " << s.id << "\n"
                << "姓名: " << s.name << "\n"
                << "年龄: " << s.get_age() << "\n"
                << "生日: " << s.birthdate.year << "-"
                << s.birthdate.month << "-"
                << s.birthdate.day << "\n"
                << "入学年份: " << s.admissionYear << "\n"
                << "专业: " << s.major << "\n"
                << "电话: " << s.contactInfo.phone
                << ", 邮箱: " << s.contactInfo.email << "\n"
                << "地址: " << s.address.province
                << "省, " << s.address.city << "市\n"
                << "在读状态: "
                << (s.status == Status::Active
                        ? "在读"
                        : s.status == Status::Leave
                              ? "休学"
                              : "毕业")
                << "\n";
        return os;
    }
};

// -- JSON conversions --

namespace nlohmann {
    inline void to_json(json& j, const Date& d) {
        j = json{
                {"year", d.year},
                {"month", d.month},
                {"day", d.day}
                };
    }

    inline void from_json(const json& j, Date& d) {
        j.at("year").get_to(d.year);
        j.at("month").get_to(d.month);
        j.at("day").get_to(d.day);
    }

    inline void to_json(json& j, const Address& a) {
        j = json{
                {"city", a.city},
                {"province", a.province}
                };
    }

    inline void from_json(const json& j, Address& a) {
        j.at("city").get_to(a.city);
        j.at("province").get_to(a.province);
    }

    inline void to_json(json& j, const Contact& c) {
        j = json{
                {"phone", c.phone},
                {"email", c.email}
                };
    }

    inline void from_json(const json& j, Contact& c) {
        j.at("phone").get_to(c.phone);
        j.at("email").get_to(c.email);
    }

    inline void to_json(json& j, const FamilyMember& fm) {
        j = json{
                {"name", fm.name},
                {"relationship", fm.relationship},
                {"contactInfo", fm.contactInfo}
                };
    }

    inline void from_json(const json& j, FamilyMember& fm) {
        j.at("name").get_to(fm.name);
        j.at("relationship").get_to(fm.relationship);
        j.at("contactInfo").get_to(fm.contactInfo);
    }

    inline void to_json(json& j, const Status& s) {
        static const char* names[] = {"Active", "Leave", "Graduated"};
        j                          = names[static_cast<int>(s)];
    }

    inline void from_json(const json& j, Status& s) {
        std::string v = j.get<std::string>();
        if (v == "Leave") s = Status::Leave;
        else if (v == "Graduated") s = Status::Graduated;
        else s                       = Status::Active;
    }

    inline void to_json(json& j, const Student& stu) {
        j = json{
                {"id", stu.get_id()},
                {"name", stu.get_name()},
                {"sex", stu.get_sex()},
                {"birthdate", stu.get_birthdate()},
                {"admissionYear", stu.get_admissionYear()},
                {"major", stu.get_major()},
                {"courses", stu.get_courses()},
                {"contact", stu.get_contact()},
                {"address", stu.get_address()},
                {"status", stu.get_status()},
                {"familyMembers", stu.get_familyMembers()}
                };
    }
    inline void from_json(const json& j, Student& stu) {
        long id;                        j.at("id").get_to(id);                  stu.set_id(id);
        std::string name;               j.at("name").get_to(name);              stu.set_name(name);
        std::string sex;                j.at("sex").get_to(sex);                stu.set_sex(sex);
        Date d;                         j.at("birthdate").get_to(d);            stu.set_birthdate(d);
        int ay;                         j.at("admissionYear").get_to(ay);       stu.set_admissionYear(ay);
        std::string maj;                j.at("major").get_to(maj);              stu.set_major(maj);
        std::vector<std::string> crs;   j.at("courses").get_to(crs);            stu.get_courses().clear();
        for (auto& c : crs)      stu.add_course(c);
        Contact ct;                     j.at("contact").get_to(ct);             stu.set_contact(ct);
        Address ad;                     j.at("address").get_to(ad);             stu.set_address(ad);
        Status st;                      j.at("status").get_to(st);              stu.set_status(st);
        std::vector<FamilyMember> fms;  j.at("familyMembers").get_to(fms);      stu.set_familyMembers(fms);
    }
}
