#ifndef WEBBRIDGE_H
#define WEBBRIDGE_H

#include <QObject>
#include <QJsonObject>

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

signals:
    void fileOpened(const QString &content);
    void fileSaved(bool success);

private:
    QString m_lastDirectory;
};

#endif // WEBBRIDGE_H