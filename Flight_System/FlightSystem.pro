QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

# 生成的可执行文件名
TARGET = FlightSystem
TEMPLATE = app

# =========================================================================
# 1. 头文件列表 (HEADERS)
# 这里的 bannerwidget.h 和 homeform.h 是你从新项目带过来的
# =========================================================================
HEADERS += \
    AdminWidget.h \
    LoginWidget.h \
    MainWindow.h \
    RegisterWidget.h \
    LoginFunc.h \
    ODBC.h \
    adbanner.h \
    bannerwidget.h \
    homeform.h \
    UserCenter.h

# =========================================================================
# 2. 源文件列表 (SOURCES)
# 对应的 cpp 文件也要加上
# =========================================================================
SOURCES += \
    AdminWidget.cpp \
    adbanner.cpp \
    main.cpp \
    LoginWidget.cpp \
    MainWindow.cpp \
    RegisterWidget.cpp \
    LoginFunc.cpp \
    ODBC.cpp \
    bannerwidget.cpp \
    homeform.cpp \
    UserCenter.cpp

# =========================================================================
# 3. 界面文件列表 (FORMS)
# homeform.ui 是新界面的一部分，必须加上
# =========================================================================
FORMS += \
    AdminWidget.ui \
    MainWindow.ui \
    LoginWidget.ui \
    RegisterWidget.ui \
    homeform.ui \
    UserCenter.ui

# =========================================================================
# 4. 资源文件
# =========================================================================
RESOURCES += \
    res.qrc
