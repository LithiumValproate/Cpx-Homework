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