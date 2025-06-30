#include "webbridge.h"
#include "stu_with_score.h"

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
#include <QWidget>

// 构造函数
WebBridge::WebBridge(QObject *parent)
    : QObject(parent)
{
    m_parentWidget = qobject_cast<QWidget*>(parent);
    load_students(); // 应用启动时加载学生数据
}

void WebBridge::open_file_dialog(const QString &title, const QString &filter)
{
    QString filePath = QFileDialog::getOpenFileName(m_parentWidget, title, "", filter);
    if (!filePath.isEmpty()) {
        emit fileSelected(filePath);
        load_students_from_file(filePath);
    }
}

void WebBridge::save_file_dialog(const QString &title, const QString &filter)
{
    QString filePath = QFileDialog::getSaveFileName(m_parentWidget, title, "", filter);
    if (!filePath.isEmpty()) {
        emit fileSaveRequested(filePath);
        save_students_to_file(filePath);
    }
}

void WebBridge::log_message(const QString &message)
{
    qDebug() << "[WEB LOG] " << message;
}

void WebBridge::show_notification(const QString &title, const QString &message)
{
    QSystemTrayIcon *trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(QIcon(":/icons/app_icon.png"));
    trayIcon->show();
    trayIcon->showMessage(title, message, QSystemTrayIcon::Information, 3000);
}

void WebBridge::minimize_to_tray()
{
    if (m_parentWidget) {
        m_parentWidget->hide();
    }
}

QJsonObject WebBridge::get_app_info()
{
    QJsonObject info;
    info["appName"] = "QtWebStudentSys";
    info["version"] = "1.0.0";
    info["qtVersion"] = qVersion();
    return info;
}

void WebBridge::add_student(const QJsonObject &studentData)
{
    try {
        // 直接使用Qt JSON转换函数
        Stu_withScore student = stuWithScoreFromQJson(studentData);
        m_students.push_back(student);
        log_message("Student " + QString::fromStdString(student.get_name()) + " added.");
        show_notification("成功", "学生 " + QString::fromStdString(student.get_name()) + " 已添加。");
        save_students(); // 自动保存
    } catch (const std::exception& e) {
        log_message(QString("添加学生失败: %1").arg(e.what()));
    } catch (...) {
        log_message("添加学生失败 (未知错误)。");
    }
}

QJsonArray WebBridge::get_students()
{
    QJsonArray studentsArray;
    for (const auto &student : m_students) {
        // 直接使用Qt JSON转换函数
        studentsArray.append(stuWithScoreToQJson(student));
    }
    return studentsArray;
}

void WebBridge::update_student(const QJsonObject &studentData)
{
    if (!studentData.contains("id")) {
        log_message("更新失败: 学生数据缺少 'id' 字段。");
        return;
    }
    long id = studentData["id"].toVariant().toLongLong();

    auto it = std::find_if(m_students.begin(), m_students.end(),
                           [id](const Stu_withScore &s) { return s.get_id() == id; });

    if (it != m_students.end()) {
        try {
            // 直接使用Qt JSON转换函数
            *it = stuWithScoreFromQJson(studentData);
            log_message("学生 " + QString::number(id) + " 的信息已更新。");
            show_notification("成功", "学生信息已更新。");
            save_students(); // 自动保存
        } catch (const std::exception& e) {
            log_message(QString("更新学生失败: %1").arg(e.what()));
        }
    } else {
        log_message("更新失败: 未找到 ID 为 " + QString::number(id) + " 的学生。");
    }
}

void WebBridge::delete_student(long studentId)
{
    auto it = std::remove_if(m_students.begin(), m_students.end(),
                             [studentId](const Stu_withScore &s) { return s.get_id() == studentId; });

    if (it != m_students.end()) {
        m_students.erase(it, m_students.end());
        log_message("学生 " + QString::number(studentId) + " 已被删除。");
        show_notification("成功", "学生已删除。");
        save_students(); // 自动保存
    }
}

QString WebBridge::get_backup_path() const
{
    QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir dir(path);
    if (!dir.exists()) {
        dir.mkpath(".");
    }
    return path + "/student_data_backup.json";
}

void WebBridge::save_students()
{
    save_students_to_file(get_backup_path());
    log_message("数据已自动备份。");
}

void WebBridge::load_students()
{
    load_students_from_file(get_backup_path());
    log_message("数据已从备份恢复。");
}

void WebBridge::save_students_to_file(const QString &filePath)
{
    // 将所有学生转换为QJsonArray
    QJsonArray studentsArray;
    for (const auto &student : m_students) {
        studentsArray.append(stuWithScoreToQJson(student));
    }

    // 创建JSON文档并保存
    QJsonDocument doc(studentsArray);
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(doc.toJson(QJsonDocument::Indented));
        file.close();
    } else {
        log_message("无法保存文件: " + filePath);
    }
}

void WebBridge::load_students_from_file(const QString &filePath)
{
    QFile file(filePath);
    if (!file.exists() || !file.open(QIODevice::ReadOnly)) {
        log_message("无法读取文件，或文件不存在: " + filePath);
        return;
    }

    QByteArray data = file.readAll();
    file.close();

    if (data.isEmpty()) {
        log_message("文件为空: " + filePath);
        return;
    }

    try {
        // 使用Qt JSON解析
        QJsonParseError error;
        QJsonDocument doc = QJsonDocument::fromJson(data, &error);
        if (error.error != QJsonParseError::NoError) {
            log_message(QString("JSON解析错误: %1").arg(error.errorString()));
            return;
        }

        m_students.clear();
        QJsonArray studentsArray = doc.array();
        for (const auto& value : studentsArray) {
            if (value.isObject()) {
                try {
                    Stu_withScore student = stuWithScoreFromQJson(value.toObject());
                    m_students.push_back(student);
                } catch (const std::exception& e) {
                    log_message(QString("解析学生数据失败: %1").arg(e.what()));
                }
            }
        }
    } catch (const std::exception& e) {
        m_students.clear();
        log_message(QString("无法解析JSON文件 '%1': %2").arg(filePath, e.what()));
    }
}