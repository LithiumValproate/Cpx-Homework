#ifndef WEBBRIDGE_H
#define WEBBRIDGE_H

#include <QObject>
#include <QJsonObject>
#include "stu_with_score.h"

class WebBridge : public QObject
{
    Q_OBJECT

public:
    explicit WebBridge(QObject *parent = nullptr);

public slots:
    QString openFileDialog(const QString &caption = QString(), const QString &filter = QString());
    QString saveFileDialog(const QString &caption = QString(), const QString &filter = QString());
    bool saveFile(const QString &filePath, const QString &content);
    QString loadFile(const QString &filePath);
    void showNotification(const QString &title, const QString &message);
    QJsonObject getAppInfo();
    void addStudent(const QJsonObject &studentData);
    QJsonArray getStudents();

signals:
    void fileOpened(const QString &content);
    void fileSaved(bool success);
    void studentDataChanged();

private:
    QString m_lastDirectory;
    std::vector<Stu_withScore> m_students;
};

#endif // WEBBRIDGE_H