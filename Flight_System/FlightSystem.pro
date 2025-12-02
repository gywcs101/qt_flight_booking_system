QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

# 2. 生成的可执行文件名
TARGET = FlightSystem
TEMPLATE = app

# 3. 源文件列表 (C++逻辑)
SOURCES += \
    main.cpp \
    MainWindow.cpp \
    LoginWidget.cpp \
    RegisterWidget.cpp \
    ConnectionManager.cpp

# 4. 头文件列表 (声明)
HEADERS += \
    MainWindow.h \
    LoginWidget.h \
    RegisterWidget.h \
    ConnectionManager.h

# 5. 界面文件列表 (设计师UI)
FORMS += \
    MainWindow.ui \
    LoginWidget.ui \
    RegisterWidget.ui

RESOURCES += \
    res.qrc
