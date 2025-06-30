#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWebEngineView>
#include <QWebChannel>
#include "webbridge.h"

class QLabel;
class QProgressBar;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onLoadStarted();
    void onLoadProgress(int progress);
    void onLoadFinished(bool success);
    void refreshPage();
    void showAbout();

private:
    void setupUI();
    void setupMenuBar();
    void setupStatusBar();
    void setupWebView();

    QWebEngineView *m_webView;
    QWebChannel *m_webChannel;
    WebBridge *m_webBridge;
    
    QLabel *m_statusLabel;
    QProgressBar *m_progressBar;
};

#endif // MAINWINDOW_H