#pragma once

#include <QJsonArray>
#include <QJsonObject>
#include <QObject>
#include <vector>
#include "stu_with_score.h"


class WebBridge : public QObject {
    Q_OBJECT

public:
    explicit WebBridge(QObject* parent = nullptr);

    signals :


    void fileSelected(const QString& filePath);

    void fileSaveRequested(const QString& filePath);

public
    slots :


    void open_file_dialog(const QString& title, const QString& filter);

    void save_file_dialog(const QString& title, const QString& filter);

    void log_message(const QString& message);

    void show_notification(const QString& title, const QString& message);

    void minimize_to_tray();

    QJsonObject get_app_info();

    void add_student(const QJsonObject& studentData);

    QJsonArray get_students();

    void update_student(const QJsonObject& studentData);

    void delete_student(long studentId);

    void save_students();

    void load_students();

private:
    void save_students_to_file(const QString& filePath);

    void load_students_from_file(const QString& filePath);

    QString get_backup_path() const;

    QWidget* m_parentWidget; // 用于弹窗的父窗口指针
    std::vector<Stu_withScore> m_students;
};
