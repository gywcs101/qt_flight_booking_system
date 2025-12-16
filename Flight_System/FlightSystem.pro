QT       += core gui sql widgets printsupport

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
    ChangeFlightDialog.h \
    DateSelector.h \
    DetailDialog.h \
    DiscoveryPage.h \
    FlightCard.h \
    FlightData.h \
    LoginWidget.h \
    MainWindow.h \
    MyOrdersPage.h \
    OrderCard.h \
    PostCard.h \
    PostData.h \
    RegisterWidget.h \
    LoginFunc.h \
    ODBC.h \
    #ResetPasswordWidget.h \
    UserSession.h \
    adbanner.h \
    flightmapwidgets.h \
    homeform.h \
    UserCenter.h \
    FavoritesPage.h \
    specialofferspage.h

# =========================================================================
# 2. 源文件列表 (SOURCES)
# =========================================================================
SOURCES += \
    AdminWidget.cpp \
    AllFlightsPage.cpp \
    ChangeFlightDialog.cpp \
    DateSelector.cpp \
    DetailDialog.cpp \
    DiscoveryPage.cpp \
    FlightCard.cpp \
    MyOrdersPage.cpp \
    OrderCard.cpp \
    PostCard.cpp \
    #ResetPasswordWidget.cpp \
    flightmapwidgets.cpp \
    main.cpp \
    LoginWidget.cpp \
    MainWindow.cpp \
    RegisterWidget.cpp \
    LoginFunc.cpp \
    ODBC.cpp \
    adbanner.cpp \
    homeform.cpp \
    UserCenter.cpp \
    FavoritesPage.cpp \
    specialofferspage.cpp

# =========================================================================
# 3. 界面文件列表 (FORMS)
# =========================================================================
FORMS += \
    AdminWidget.ui \
    AllFlightsPage.ui \
    ChangeFlightDialog.ui \
    DiscoveryPage.ui \
    MainWindow.ui \
    LoginWidget.ui \
    MyOrdersPage.ui \
    RegisterWidget.ui \
    #ResetPasswordWidget.ui \
    homeform.ui \
    UserCenter.ui \
    FavoritesPage.ui

# =========================================================================
# 4. 资源文件
# =========================================================================
RESOURCES += \
    res.qrc
