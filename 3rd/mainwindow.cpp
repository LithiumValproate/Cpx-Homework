#include "mainwindow.h"

#include <QApplication>
#include <QLabel>
#include <QMenuBar>
#include <QMessageBox>
#include <QProgressBar>
#include <QStatusBar>
#include <QUrl>
#include <QVBoxLayout>
#include <QWebChannel>
#include <QWebEngineSettings>
#include <QWebEngineView>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
      , m_webView(nullptr)
      , m_webChannel(nullptr)
      , m_webBridge(nullptr) {
    setup_ui();
    setup_menu_bar();
    setup_status_bar();
    setup_web_view();

    setWindowTitle("Qt Web学生管理系统");
    resize(1400, 900);
}

MainWindow::~MainWindow() = default;

void MainWindow::setup_ui() {
    QWidget* centralWidget = new QWidget;
    setCentralWidget(centralWidget);

    QVBoxLayout* layout = new QVBoxLayout(centralWidget);
    layout->setContentsMargins(0, 0, 0, 0);

    m_webView = new QWebEngineView;
    layout->addWidget(m_webView);
}

void MainWindow::setup_menu_bar() {
    QMenu* fileMenu = menuBar()->addMenu("文件(&F)");

    QAction* refreshAction = new QAction("刷新(&R)", this);
    refreshAction->setShortcut(QKeySequence::Refresh);
    connect(refreshAction, &QAction::triggered, this, &MainWindow::refresh_page);
    fileMenu->addAction(refreshAction);

    fileMenu->addSeparator();

    QAction* exitAction = new QAction("退出(&X)", this);
    exitAction->setShortcut(QKeySequence::Quit);
    connect(exitAction, &QAction::triggered, this, &QWidget::close);
    fileMenu->addAction(exitAction);

    QMenu* helpMenu      = menuBar()->addMenu("帮助(&H)");
    QAction* aboutAction = new QAction("关于(&A)", this);
    connect(aboutAction, &QAction::triggered, this, &MainWindow::show_about);
    helpMenu->addAction(aboutAction);
}

void MainWindow::setup_status_bar() {
    m_statusLabel = new QLabel("就绪");
    statusBar()->addWidget(m_statusLabel);

    m_progressBar = new QProgressBar;
    m_progressBar->setVisible(false);
    m_progressBar->setMaximumWidth(200);
    statusBar()->addPermanentWidget(m_progressBar);
}

void MainWindow::setup_web_view() {
    QWebEngineSettings* settings = m_webView->settings();
    settings->setAttribute(QWebEngineSettings::LocalStorageEnabled, true);
    settings->setAttribute(QWebEngineSettings::JavascriptEnabled, true);

    connect(m_webView, &QWebEngineView::loadStarted, this, &MainWindow::on_load_started);
    connect(m_webView, &QWebEngineView::loadProgress, this, &MainWindow::on_load_progress);
    connect(m_webView, &QWebEngineView::loadFinished, this, &MainWindow::on_load_finished);

    // 设置Web通信
    m_webChannel = new QWebChannel(this);
    m_webBridge  = new WebBridge(this);
    m_webChannel->registerObject("qtBridge", m_webBridge);
    m_webView->page()->setWebChannel(m_webChannel);

    // 加载HTML文件
    m_webView->load(QUrl("qrc:/web/web/index.html"));
}

void MainWindow::on_load_started() {
    m_progressBar->setVisible(true);
    m_statusLabel->setText("正在加载...");
}

void MainWindow::on_load_progress(int progress) {
    m_progressBar->setValue(progress);
}

void MainWindow::on_load_finished(bool success) {
    m_progressBar->setVisible(false);

    if (success) {
        m_statusLabel->setText("加载完成");
    } else {
        m_statusLabel->setText("加载失败");
        QMessageBox::warning(this, "错误", "无法加载Web页面，请检查web目录下的HTML文件是否存在。");
    }
}

void MainWindow::refresh_page() {
    m_webView->reload();
}

void MainWindow::show_about() {
    QMessageBox::about(this, "关于",
                       QString("Qt Web学生管理系统\n版本: %1\n\n基于Qt WebEngine的学生管理系统")
                       .arg(QApplication::applicationVersion()));
}
