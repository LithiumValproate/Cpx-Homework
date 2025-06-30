#pragma once

#include "webbridge.h"

#include <QMainWindow>
#include <QWebEngineView>
#include <QWebChannel>

class QLabel;
class QProgressBar;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);

    ~MainWindow() override;

private slots:
    void on_load_started();

    void on_load_progress(int progress);

    void on_load_finished(bool success);

    void refresh_page();

    void show_about();

private:
    void setup_ui();

    void setup_menu_bar();

    void setup_status_bar();

    void setup_web_view();

    QWebEngineView* m_webView;
    QWebChannel* m_webChannel;
    WebBridge* m_webBridge;

    QLabel* m_statusLabel;
    QProgressBar* m_progressBar;
};
