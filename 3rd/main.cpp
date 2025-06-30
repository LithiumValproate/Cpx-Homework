#include "mainwindow.h"

#include <QApplication>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    app.setApplicationName("Qt Web学生管理系统");
    app.setApplicationVersion("1.0.0");
    app.setOrganizationName("Qt学习项目");
    
    app.setStyle(QStyleFactory::create("Fusion"));
    
    QString styleSheet = R"(
        QMainWindow {
            background-color: #f5f7fa;
        }
        QMenuBar {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1, 
                stop:0 #667eea, stop:1 #764ba2);
            color: white;
            padding: 4px;
        }
        QMenuBar::item {
            background-color: transparent;
            padding: 8px 12px;
            border-radius: 4px;
        }
        QMenuBar::item:selected {
            background-color: rgba(255, 255, 255, 0.2);
        }
        QToolBar {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1, 
                stop:0 #667eea, stop:1 #764ba2);
            border: none;
            spacing: 8px;
            padding: 4px;
        }
        QToolButton {
            background-color: rgba(255, 255, 255, 0.2);
            border: none;
            color: white;
            padding: 8px 12px;
            border-radius: 4px;
            font-weight: bold;
        }
        QToolButton:hover {
            background-color: rgba(255, 255, 255, 0.3);
        }
        QStatusBar {
            background-color: #e9ecef;
            border-top: 1px solid #dee2e6;
        }
    )";
    app.setStyleSheet(styleSheet);
    
    MainWindow window;
    window.show();
    
    return app.exec();
}