#ifndef WEBBRIDGE_H
#define WEBBRIDGE_H

#include <QObject>
#include <QJsonObject>
#include <QJsonArray>
#include <vector>
#include "stu_with_score.h"

// 前向声明 QWidget
class QWidget;

class WebBridge : public QObject
{
    Q_OBJECT

public:
    // *** 修改这里：将 QWidget* 改为 QObject* ***
    explicit WebBridge(QObject *parent = nullptr);

    signals:
        void fileSelected(const QString &filePath);
    void fileSaveRequested(const QString &filePath);

public slots:
    void openFileDialog(const QString &title, const QString &filter);
    void saveFileDialog(const QString &title, const QString &filter);
    void logMessage(const QString &message);
    void showNotification(const QString &title, const QString &message);
    void minimizeToTray();
    QJsonObject getAppInfo();
    void addStudent(const QJsonObject &studentData);
    QJsonArray getStudents();
    void updateStudent(const QJsonObject &studentData);
    void deleteStudent(long studentId);
    void saveStudents();
    void loadStudents();

private:
    void saveStudentsToFile(const QString &filePath);
    void loadStudentsFromFile(const QString &filePath);
    QString getBackupPath() const;

    QWidget *m_parentWidget; // 用于弹窗的父窗口指针
    std::vector<Stu_withScore> m_students;
};

#endif // WEBBRIDGE_H