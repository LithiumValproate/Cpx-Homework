#include "webbridge.h"
#include "stu_with_score.h" // 包含你的学生类定义

#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QStandardPaths>
#include <QSystemTrayIcon>
#include <QWidget> // 需要包含 QWidget 以使用 qobject_cast

// -- 辅助函数：在 Qt JSON 和 nlohmann::json 之间转换 --

// 将 QJsonObject 转换为 nlohmann::json
nlohmann::json qJsonObjectToNlohmannJson(const QJsonObject& qJsonObj) {
    // 将 QJsonObject 转换为紧凑的 JSON 字符串（std::string）
    QJsonDocument doc(qJsonObj);
    std::string str = doc.toJson(QJsonDocument::Compact).toStdString();
    // 解析字符串为 nlohmann::json 对象
    return nlohmann::json::parse(str);
}

// 将 nlohmann::json 转换为 QJsonObject
QJsonObject nlohmannJsonToQJsonObject(const nlohmann::json& nlohmannJson) {
    // 将 nlohmann::json 对象转储为 std::string
    std::string str = nlohmannJson.dump();
    // 从字符串解析为 QJsonDocument，然后获取 QJsonObject
    QJsonDocument doc = QJsonDocument::fromJson(QString::fromStdString(str).toUtf8());
    return doc.object();
}

// --------------------------------------------------------------------

// 构造函数：接受 QObject* 以解决链接器错误
WebBridge::WebBridge(QObject *parent)
    : QObject(parent)
{
    // 尝试将 QObject* 父级安全地转换为 QWidget*
    // 这对于需要一个窗口作为父级的对话框（如 QFileDialog）是必需的
    // 如果转换失败，m_parentWidget 将为 nullptr
    m_parentWidget = qobject_cast<QWidget*>(parent);
    loadStudents(); // 应用启动时加载学生数据
}

void WebBridge::openFileDialog(const QString &title, const QString &filter)
{
    // 使用 m_parentWidget 作为文件对话框的父窗口
    QString filePath = QFileDialog::getOpenFileName(m_parentWidget, title, "", filter);
    if (!filePath.isEmpty()) {
        emit fileSelected(filePath); // 发出文件已选择的信号
        loadStudentsFromFile(filePath); // 从选择的文件加载学生
    }
}

void WebBridge::saveFileDialog(const QString &title, const QString &filter)
{
    QString filePath = QFileDialog::getSaveFileName(m_parentWidget, title, "", filter);
    if (!filePath.isEmpty()) {
        emit fileSaveRequested(filePath); // 发出保存请求信号
        saveStudentsToFile(filePath); // 将当前学生数据保存到文件
    }
}

void WebBridge::logMessage(const QString &message)
{
    qDebug() << "[WEB LOG] " << message;
}

void WebBridge::showNotification(const QString &title, const QString &message)
{
    QSystemTrayIcon *trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(QIcon(":/icons/app_icon.png")); // 确保你的资源文件中有这个图标
    trayIcon->show();
    trayIcon->showMessage(title, message, QSystemTrayIcon::Information, 3000);
}

void WebBridge::minimizeToTray()
{
    if (m_parentWidget) {
        m_parentWidget->hide();
    }
}

QJsonObject WebBridge::getAppInfo()
{
    QJsonObject info;
    info["appName"] = "QtWebStudentSys";
    info["version"] = "1.0.0";
    info["qtVersion"] = qVersion();
    return info;
}

void WebBridge::addStudent(const QJsonObject &studentData)
{
    try {
        // 使用辅助函数将 Qt JSON 转换为 nlohmann::json
        nlohmann::json j = qJsonObjectToNlohmannJson(studentData);
        // 利用 nlohmann::json 的 from_json 功能直接创建 Stu_withScore 对象
        Stu_withScore student = j.get<Stu_withScore>();
        m_students.push_back(student);
        logMessage("Student " + QString::fromStdString(student.get_name()) + " added.");
        showNotification("成功", "学生 " + QString::fromStdString(student.get_name()) + " 已添加。");
        saveStudents(); // 自动保存
    } catch (const nlohmann::json::exception& e) {
        logMessage(QString("添加学生失败 (JSON 错误): %1").arg(e.what()));
    } catch (...) {
        logMessage("添加学生失败 (未知错误)。");
    }
}

QJsonArray WebBridge::getStudents()
{
    QJsonArray studentsArray;
    for (const auto &student : m_students) {
        // 利用 nlohmann::json 的 to_json 功能将学生对象转换为 JSON
        nlohmann::json j = student;
        // 使用辅助函数将 nlohmann::json 转换为 QJsonObject 并添加到数组
        studentsArray.append(nlohmannJsonToQJsonObject(j));
    }
    return studentsArray;
}

void WebBridge::updateStudent(const QJsonObject &studentData)
{
    if (!studentData.contains("id")) {
        logMessage("更新失败: 学生数据缺少 'id' 字段。");
        return;
    }
    long id = studentData["id"].toVariant().toLongLong();

    auto it = std::find_if(m_students.begin(), m_students.end(),
                           [id](const Stu_withScore &s) { return s.get_id() == id; });

    if (it != m_students.end()) {
        try {
            nlohmann::json j = qJsonObjectToNlohmannJson(studentData);
            *it = j.get<Stu_withScore>(); // 直接用新数据覆盖旧的学生对象
            logMessage("学生 " + QString::number(id) + " 的信息已更新。");
            showNotification("成功", "学生信息已更新。");
            saveStudents(); // 自动保存
        } catch (const nlohmann::json::exception& e) {
            logMessage(QString("更新学生失败 (JSON 错误): %1").arg(e.what()));
        }
    } else {
        logMessage("更新失败: 未找到 ID 为 " + QString::number(id) + " 的学生。");
    }
}

void WebBridge::deleteStudent(long studentId)
{
    auto it = std::remove_if(m_students.begin(), m_students.end(),
                             [studentId](const Stu_withScore &s) { return s.get_id() == studentId; });

    if (it != m_students.end()) {
        m_students.erase(it, m_students.end());
        logMessage("学生 " + QString::number(studentId) + " 已被删除。");
        showNotification("成功", "学生已删除。");
        saveStudents(); // 自动保存
    }
}

QString WebBridge::getBackupPath() const
{
    // 获取应用程序数据目录
    QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir dir(path);
    if (!dir.exists()) {
        dir.mkpath("."); // 如果目录不存在，则创建它
    }
    return path + "/student_data_backup.json";
}

void WebBridge::saveStudents()
{
    saveStudentsToFile(getBackupPath());
    logMessage("数据已自动备份。");
}

void WebBridge::loadStudents()
{
    loadStudentsFromFile(getBackupPath());
    logMessage("数据已从备份恢复。");
}

void WebBridge::saveStudentsToFile(const QString &filePath)
{
    // 将整个学生向量转换为 nlohmann::json 数组
    nlohmann::json j = m_students;
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly)) {
        // 使用 dump(2) 进行格式化输出（带缩进），便于阅读
        file.write(QString::fromStdString(j.dump(2)).toUtf8());
        file.close();
    } else {
        logMessage("无法保存文件: " + filePath);
    }
}

void WebBridge::loadStudentsFromFile(const QString &filePath)
{
    QFile file(filePath);
    if (!file.exists() || !file.open(QIODevice::ReadOnly)) {
        logMessage("无法读取文件，或文件不存在: " + filePath);
        return;
    }

    QByteArray data = file.readAll();
    file.close();

    if (data.isEmpty()) {
        logMessage("文件为空: " + filePath);
        return;
    }

    try {
        // 解析整个文件内容为 nlohmann::json 对象
        nlohmann::json j = nlohmann::json::parse(data.toStdString());
        // 将 JSON 数组转换为学生向量
        m_students = j.get<std::vector<Stu_withScore>>();
    } catch (const nlohmann::json::exception& e) {
        m_students.clear(); // 解析失败时清空数据，防止脏数据
        logMessage(QString("无法解析JSON文件 '%1': %2").arg(filePath, e.what()));
    }
}