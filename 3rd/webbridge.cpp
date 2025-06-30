#include "webbridge.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QApplication>
#include <QStandardPaths>
#include <QTextStream>
#include <QDateTime>

WebBridge::WebBridge(QObject *parent)
    : QObject(parent)
    , m_lastDirectory(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation))
{
}

QString WebBridge::openFileDialog(const QString &caption, const QString &filter)
{
    QString fileName = QFileDialog::getOpenFileName(
        nullptr,
        caption.isEmpty() ? "选择文件" : caption,
        m_lastDirectory,
        filter.isEmpty() ? "JSON文件 (*.json);;所有文件 (*.*)" : filter
    );
    
    if (!fileName.isEmpty()) {
        QFileInfo fileInfo(fileName);
        m_lastDirectory = fileInfo.absolutePath();
    }
    
    return fileName;
}

QString WebBridge::saveFileDialog(const QString &caption, const QString &filter)
{
    QString fileName = QFileDialog::getSaveFileName(
        nullptr,
        caption.isEmpty() ? "保存文件" : caption,
        m_lastDirectory + "/students_" + QDateTime::currentDateTime().toString("yyyy-MM-dd") + ".json",
        filter.isEmpty() ? "JSON文件 (*.json);;所有文件 (*.*)" : filter
    );
    
    if (!fileName.isEmpty()) {
        QFileInfo fileInfo(fileName);
        m_lastDirectory = fileInfo.absolutePath();
    }
    
    return fileName;
}

bool WebBridge::saveFile(const QString &filePath, const QString &content)
{
    if (filePath.isEmpty()) return false;
    
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }
    
    QTextStream out(&file);
    out << content;
    
    return true;
}

QString WebBridge::loadFile(const QString &filePath)
{
    if (filePath.isEmpty()) return QString();
    
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return QString();
    }
    
    QTextStream in(&file);
    return in.readAll();
}

void WebBridge::showNotification(const QString &title, const QString &message)
{
    QMessageBox::information(nullptr, title, message);
}

QJsonObject WebBridge::getAppInfo()
{
    QJsonObject info;
    info["name"] = QApplication::applicationName();
    info["version"] = QApplication::applicationVersion();
    info["organization"] = QApplication::organizationName();
    
    return info;
}

// 从网页接收一个学生JSON对象，并添加到vector中
void WebBridge::addStudent(const QJsonObject &studentData)
{
    // 这里需要你自己实现从 QJsonObject 到 Stu_withScore 的转换
    // 为了简化，我们只读取几个关键字段作为示例
    // 你需要根据 student.h 中的 from_json 来完善它
    try {
        Stu_withScore student;
        student.set_id(studentData["id"].toVariant().toLongLong());
        student.set_name(studentData["name"].toString());
        student.set_major(studentData["major"].toString());

        // 你可以继续设置其他属性...

        m_students.push_back(student);
        emit studentDataChanged(); // 通知UI更新
        showNotification("成功", "学生 " + student.get_name() + " 已添加。");

    } catch (...) {
        showNotification("错误", "添加学生失败，请检查数据格式。");
    }
}

// 将C++ vector中的所有学生数据转换成JSON数组返回给网页
QJsonArray WebBridge::getStudents()
{
    QJsonArray studentsArray;
    for (const auto& student : m_students) {
        // 这里需要你自己实现从 Stu_withScore 到 QJsonObject 的转换
        // 为了简化，我们只转换几个关键字段作为示例
        // 你需要根据 student.h 中的 to_json 来完善它
        QJsonObject studentObj;
        studentObj["id"] = QJsonValue::fromVariant(QVariant::fromValue(student.get_id()));
        studentObj["name"] = student.get_name();
        studentObj["major"] = student.get_major();
        studentObj["age"] = student.get_age();

        studentsArray.append(studentObj);
    }
    return studentsArray;
}