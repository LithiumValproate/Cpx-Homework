#pragma once

#include <algorithm>
#include <ctime>
#include <ostream>
#include <string>
#include <vector>

#ifdef USE_QTJSON
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QString>
#endif


// -- data types --

struct Date {
    int year{};
    int month{};
    int day{};

    Date() = default;

    Date(int y, int m, int d) : year(y), month(m), day(d) {}
};

struct Address {
    std::string province;
    std::string city;

    Address() = default;

    Address(const std::string& p, const std::string& c)
        : province(p), city(c) {}
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

enum class Sex { Male = 0, Female = 1 };

enum class Status { Active = 0, Leave = 1, Graduated = 2 };

class Student {
private:
    long id{};
    std::string name;
    Sex sex;
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
            Sex s,
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

    auto get_sex() const -> Sex { return sex; }
    void set_sex(Sex v) { sex = v; }

    auto get_birthdate() const -> const Date& { return birthdate; }
    void set_birthdate(const Date& v) { birthdate = v; }

    auto get_admission_year() const -> int { return admissionYear; }
    void set_admission_year(int v) { admissionYear = v; }

    auto get_major() const -> const std::string& { return major; }
    void set_major(const std::string& v) { major = v; }

    auto get_courses() const -> const std::vector<std::string>& { return courses; }
    void add_course(const std::string& c) { courses.push_back(c); }

    void del_course(const std::string& c) {
        courses.erase(
                      std::remove(courses.begin(), courses.end(), c),
                      courses.end());
    }

    void clear_courses() { courses.clear(); }

    auto get_contact() const & -> const Contact& { return contactInfo; }
    void set_contact(const Contact& v) { contactInfo = v; }

    auto get_address() const & -> const Address& { return address; }
    void set_address(const Address& v) { address = v; }

    auto get_family_members() const -> const std::vector<FamilyMember>& {
        return familyMembers;
    }

    void set_family_members(const std::vector<FamilyMember>& v) {
        familyMembers = v;
    }

    void add_family_member(const FamilyMember& fm) {
        familyMembers.push_back(fm);
    }

    void del_family_member(const FamilyMember& fm) {
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
                << "性别: " << (s.get_sex() == Sex::Male ? "男" : "女") << "\n"
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
#ifdef USE_QTJSON

// Sex Qt JSON conversions
inline auto sex_to_qjson_string(Sex s) -> QString {
    return s == Sex::Male ? "Male" : "Female";
}

inline auto sex_from_qjson_string(const QString& str) -> Sex {
    return str == "Female" ? Sex::Female : Sex::Male;
}

// Date Qt JSON conversions
inline auto date_to_qjson(const Date& d) -> QJsonObject {
    QJsonObject obj;
    obj["year"]  = d.year;
    obj["month"] = d.month;
    obj["day"]   = d.day;
    return obj;
}

inline auto date_from_qjson(const QJsonObject& obj) -> Date {
    Date d;
    d.year  = obj["year"].toInt();
    d.month = obj["month"].toInt();
    d.day   = obj["day"].toInt();
    return d;
}

// Address Qt JSON conversions
inline auto address_to_qjson(const Address& a) -> QJsonObject {
    QJsonObject obj;
    obj["province"] = QString::fromStdString(a.province);
    obj["city"]     = QString::fromStdString(a.city);
    return obj;
}

inline auto address_from_qjson(const QJsonObject& obj) -> Address {
    Address a;
    a.province = obj["province"].toString().toStdString();
    a.city     = obj["city"].toString().toStdString();
    return a;
}

// Contact Qt JSON conversions
inline auto contact_to_qjson(const Contact& c) -> QJsonObject {
    QJsonObject obj;
    obj["phone"] = QString::fromStdString(c.phone);
    obj["email"] = QString::fromStdString(c.email);
    return obj;
}

inline auto contact_from_qjson(const QJsonObject& obj) -> Contact {
    Contact c;
    c.phone = obj["phone"].toString().toStdString();
    c.email = obj["email"].toString().toStdString();
    return c;
}

// FamilyMember Qt JSON conversions
inline auto family_member_to_qjson(const FamilyMember& fm) -> QJsonObject {
    QJsonObject obj;
    obj["name"]         = QString::fromStdString(fm.name);
    obj["relationship"] = QString::fromStdString(fm.relationship);
    obj["contactInfo"]  = contact_to_qjson(fm.contactInfo);
    return obj;
}

inline auto family_member_from_qjson(const QJsonObject& obj) -> FamilyMember {
    FamilyMember fm;
    fm.name         = obj["name"].toString().toStdString();
    fm.relationship = obj["relationship"].toString().toStdString();
    fm.contactInfo  = contact_from_qjson(obj["contactInfo"].toObject());
    return fm;
}

// Status Qt JSON conversions
inline auto status_to_qjson_string(Status st) -> QString {
    switch (st) {
        case Status::Active: return "Active";
        case Status::Leave: return "Leave";
        case Status::Graduated: return "Graduated";
        default: return "Active";
    }
}

inline auto status_from_qjson_string(const QString& str) -> Status {
    if (str == "Leave") return Status::Leave;
    else if (str == "Graduated") return Status::Graduated;
    return Status::Active;
}

// Student Qt JSON conversions
inline auto student_to_qjson(const Student& stu) -> QJsonObject {
    QJsonObject obj;
    obj["id"]            = static_cast<qint64>(stu.get_id());
    obj["name"]          = QString::fromStdString(stu.get_name());
    obj["sex"]           = sex_to_qjson_string(stu.get_sex());
    obj["birthdate"]     = date_to_qjson(stu.get_birthdate());
    obj["admissionYear"] = stu.get_admission_year();
    obj["major"]         = QString::fromStdString(stu.get_major());
    QJsonArray coursesArray;
    for (const auto& course : stu.get_courses()) {
        coursesArray.append(QString::fromStdString(course));
    }
    obj["courses"] = coursesArray;
    obj["contact"] = contact_to_qjson(stu.get_contact());
    obj["address"] = address_to_qjson(stu.get_address());
    obj["status"]  = status_to_qjson_string(stu.get_status());
    QJsonArray familyArray;
    for (const auto& fm : stu.get_family_members()) {
        familyArray.append(family_member_to_qjson(fm));
    }
    obj["familyMembers"] = familyArray;
    return obj;
}

inline auto student_from_qjson(const QJsonObject& obj) -> Student {
    Student stu;
    stu.set_id(obj["id"].toVariant().toLongLong());
    stu.set_name(obj["name"].toString().toStdString());
    stu.set_sex(sex_from_qjson_string(obj["sex"].toString()));
    stu.set_birthdate(date_from_qjson(obj["birthdate"].toObject()));
    stu.set_admission_year(obj["admissionYear"].toInt());
    stu.set_major(obj["major"].toString().toStdString());
    stu.clear_courses();
    QJsonArray coursesArray = obj["courses"].toArray();
    for (const auto& courseValue : coursesArray) {
        stu.add_course(courseValue.toString().toStdString());
    }
    stu.set_contact(contact_from_qjson(obj["contact"].toObject()));
    stu.set_address(address_from_qjson(obj["address"].toObject()));
    stu.set_status(status_from_qjson_string(obj["status"].toString()));
    QJsonArray familyArray = obj["familyMembers"].toArray();
    std::vector<FamilyMember> familyMembers;
    for (const auto& familyValue : familyArray) {
        familyMembers.push_back(family_member_from_qjson(familyValue.toObject()));
    }
    stu.set_family_members(familyMembers);
    return stu;
}

#endif // USE_QTJSON
