#define USE_QTJSON

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
        Stu_withScore student = stu_with_score_from_qjson(studentData);
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
        studentsArray.append(stu_with_score_to_qjson(student));
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
                           [id](const Stu_withScore& s) {
                               return s.get_id() == id;
                           });

    if (it != m_students.end()) {
        try {
            *it = stu_with_score_from_qjson(studentData);
            log_message("Student " + QString::fromStdString(it->get_name()) + " updated.");
            show_notification("成功", "学生 " + QString::fromStdString(it->get_name()) + " 已更新。");
            save_students(); // 自动保存
        } catch (const std::exception& e) {
            log_message(QString("更新学生失败: %1").arg(e.what()));
        } catch (...) {
            log_message("更新学生失败 (未知错误)。");
        }
    } else {
        log_message(QString("更新失败: 未找到ID为 %1 的学生。").arg(id));
    }
}

void WebBridge::delete_student(long studentId)
{
    auto it = std::remove_if(m_students.begin(), m_students.end(),
                             [studentId](const Stu_withScore& s) {
                                 return s.get_id() == studentId;
                             });

    if (it != m_students.end()) {
        m_students.erase(it, m_students.end());
        log_message(QString("Student with ID %1 deleted.").arg(studentId));
        show_notification("成功", QString("ID为 %1 的学生已删除。").arg(studentId));
        save_students(); // 自动保存
    } else {
        log_message(QString("删除失败: 未找到ID为 %1 的学生。").arg(studentId));
    }
}

void WebBridge::save_students()
{
    save_students_to_file(get_backup_path());
}

void WebBridge::load_students()
{
    load_students_from_file(get_backup_path());
}

void WebBridge::save_students_to_file(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        log_message("无法打开文件进行写入: " + file.errorString());
        return;
    }

    QJsonArray studentsArray = get_students();
    QJsonDocument doc(studentsArray);
    file.write(doc.toJson());
    file.close();
    log_message("学生数据已保存到: " + filePath);
}

void WebBridge::load_students_from_file(const QString &filePath)
{
    QFile file(filePath);
    if (!file.exists()) {
        log_message("学生数据文件不存在: " + filePath);
        return;
    }
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        log_message("无法打开文件进行读取: " + file.errorString());
        return;
    }

    QByteArray data = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);

    if (doc.isArray()) {
        m_students.clear();
        QJsonArray studentsArray = doc.array();
        for (const auto& item : studentsArray) {
            if (item.isObject()) {
                add_student(item.toObject());
            }
        }
        log_message("学生数据已从 " + filePath + " 加载。");
    }
    file.close();
}


QString WebBridge::get_backup_path() const
{
    QString backupDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    if (!QDir(backupDir).exists()) {
        QDir().mkpath(backupDir);
    }
    return backupDir + "/student_data.json";
}