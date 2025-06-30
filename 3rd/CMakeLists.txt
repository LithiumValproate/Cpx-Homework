cmake_minimum_required(VERSION 3.16)

project(QtWebStudentSys VERSION 1.0.0 LANGUAGES CXX)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找Qt组件
find_package(Qt6 REQUIRED COMPONENTS
    Core
    Widgets
    WebEngineWidgets
    WebChannel
)

# 启用Qt的MOC、UIC、RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# 设置源文件
set(SOURCES
    main.cpp
    mainwindow.cpp
    webbridge.cpp
)

# 设置头文件
set(HEADERS
    mainwindow.h
    webbridge.h
    student.h
    stu_with_score.h
)

# 设置资源文件
set(RESOURCES
    resources.qrc
)

# 创建可执行文件
add_executable(QtWebStudentSys
    ${SOURCES}
    ${HEADERS}
    ${RESOURCES}
)

# 链接Qt库
target_link_libraries(QtWebStudentSys
    Qt6::Core
    Qt6::Widgets
    Qt6::WebEngineWidgets
    Qt6::WebChannel
)

# 设置包含目录
target_include_directories(QtWebStudentSys PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# macOS特定设置
if(APPLE)
    set_target_properties(QtWebStudentSys PROPERTIES
        MACOSX_BUNDLE TRUE
    )
endif()

# 安装设置
install(TARGETS QtWebStudentSys
    BUNDLE DESTINATION .
    RUNTIME DESTINATION bin
)

# 拷贝web资源文件到构建目录
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/web
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/icons
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# 设置输出目录
set_target_properties(QtWebStudentSys PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin
)

# 调试模式设置
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(QtWebStudentSys PRIVATE DEBUG)
endif()

# 编译选项
if(MSVC)
    target_compile_options(QtWebStudentSys PRIVATE /W4)
else()
    target_compile_options(QtWebStudentSys PRIVATE -Wall -Wextra -Wpedantic)
endif()

# 设置版本信息
target_compile_definitions(QtWebStudentSys PRIVATE
    APP_VERSION="${PROJECT_VERSION}"
)
