# Qt Web学生管理系统

将Web版学生管理系统完美集成到Qt桌面应用中，提供原生的桌面体验和增强功能。

## 🚀 项目特性

### 🖥️ Qt桌面集成
- ✅ QWebEngineView嵌入Web界面
- ✅ 原生Qt菜单栏和工具栏
- ✅ 系统托盘支持
- ✅ 快捷键操作
- ✅ 窗口状态保存

### 🔗 Web-Qt通信
- ✅ QWebChannel双向通信
- ✅ Qt原生文件对话框
- ✅ 系统通知
- ✅ 日志记录
- ✅ 应用信息获取

### 💾 数据管理增强
- ✅ 自动数据备份
- ✅ 备份/恢复功能
- ✅ 设置持久化
- ✅ 错误处理

### 🎨 用户体验
- ✅ 现代化Qt界面风格
- ✅ 响应式布局
- ✅ 状态栏信息显示
- ✅ 进度条反馈
- ✅ 全屏模式

## 📋 系统要求

- Qt 5.15+ 或 Qt 6.x
- QWebEngineView 模块
- C++17 支持
- 操作系统: Windows 10+, macOS 10.14+, Linux

## 🛠 编译说明

### 1. 环境准备
```bash
# 确保安装了Qt和QWebEngine模块
# Qt Creator或命令行编译
```

### 2. 编译步骤
```bash
# 使用Qt Creator打开项目
# 或使用命令行:
qmake QtWebStudentSys.pro
make
```

### 3. 文件准备
确保项目目录结构如下:
```
3rd/
├── QtWebStudentSystem.pro
├── main.cpp
├── mainwindow.h
├── mainwindow.cpp
├── webbridge.h
├── webbridge.cpp
├── resources.qrc
├── web/
│   ├── student-management.html
│   ├── styles.css
│   └── script.js
└── icons/
    ├── app_icon.png
    ├── export.png
    ├── import.png
    ├── refresh.png
    ├── backup.png
    ├── restore.png
    └── settings.png
```

## 🚀 使用说明

### 启动应用
1. 双击可执行文件启动
2. 应用会自动加载Web界面
3. 支持系统托盘运行

### 主要功能
- **文件操作**: Ctrl+O导入, Ctrl+Shift+S导出
- **视图操作**: F5刷新, F11全屏
- **开发工具**: F12开发者工具
- **系统托盘**: 最小化到托盘, 双击恢复

### Web-Qt交互
Web页面可以调用Qt原生功能:
```javascript
// 打开文件对话框
qtBridge.openFileDialog("选择文件", "JSON文件 (*.json)");

// 保存文件对话框
qtBridge.saveFileDialog("保存文件", "JSON文件 (*.json)");

// 显示系统通知
qtBridge.showNotification("标题", "消息内容");

// 最小化到托盘
qtBridge.minimizeToTray();

// 获取应用信息
qtBridge.getAppInfo();
```

## 🔧 开发说明

### Web桥接
- `WebBridge`类处理Web与Qt的通信
- 使用`QWebChannel`实现双向通信
- 支持文件操作、通知、日志等功能

### 设置管理
- 使用`QSettings`保存应用设置
- 窗口状态自动保存和恢复
- 支持配置文件扩展

### 系统集成
- 系统托盘支持
- 原生文件对话框
- 快捷键和菜单
- 状态栏信息显示

## 📝 更新日志

### v1.0.0
- ✅ 基础Qt集成
- ✅ Web界面嵌入
- ✅ 文件操作支持
- ✅ 系统托盘功能
- ✅ 自动备份系统

## 🤝 贡献

欢迎提交问题和改进建议！

## 📄 许可证

MIT License