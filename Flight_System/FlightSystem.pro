QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

# 生成的可执行文件名
TARGET = FlightSystem
TEMPLATE = app

# =========================================================================
# 1. 头文件列表 (HEADERS)
# =========================================================================
HEADERS += \
    AdminWidget.h \
    AllFlightsPage.h \
    DateSelector.h \
    DiscoveryPage.h \
    FlightCard.h \
    FlightData.h \
    LoginWidget.h \
    MainWindow.h \
    PostCard.h \
    RegisterWidget.h \
    LoginFunc.h \
    ODBC.h \
    UserSession.h \
    adbanner.h \
    homeform.h \
    UserCenter.h

# =========================================================================
# 2. 源文件列表 (SOURCES)
# =========================================================================
SOURCES += \
    AdminWidget.cpp \
    AllFlightsPage.cpp \
    DateSelector.cpp \
    DiscoveryPage.cpp \
    FlightCard.cpp \
    PostCard.cpp \
    main.cpp \
    LoginWidget.cpp \
    MainWindow.cpp \
    RegisterWidget.cpp \
    LoginFunc.cpp \
    ODBC.cpp \
    adbanner.cpp \
    homeform.cpp \
    UserCenter.cpp

# =========================================================================
# 3. 界面文件列表 (FORMS)
# =========================================================================
FORMS += \
    AdminWidget.ui \
    AllFlightsPage.ui \
    DiscoveryPage.ui \
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