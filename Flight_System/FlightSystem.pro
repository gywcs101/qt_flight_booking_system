QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

# 2. 生成的可执行文件名
TARGET = FlightSystem
TEMPLATE = app

# 3. 源文件列表 (C++逻辑)
HEADERS += \
    LoginWidget.h \
    MainWindow.h \
    RegisterWidget.h \
    LoginFunc.h \
    ODBC.h          # 【新增】加上这个
    # ConnectionManager.h  <-- 【删除】删掉这个

SOURCES += \
    main.cpp \
    LoginWidget.cpp \
    MainWindow.cpp \
    RegisterWidget.cpp \
    LoginFunc.cpp \
    ODBC.cpp        # 【新增】加上这个

# 5. 界面文件列表 (设计师UI)
FORMS += \
    MainWindow.ui \
    LoginWidget.ui \
    RegisterWidget.ui

RESOURCES += \
    res.qrc
