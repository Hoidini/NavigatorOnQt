QT       += core gui
QT += sql
QT += testlib
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
DEFINES += QT_NO_DEBUG_OUTPUT
CONFIG += c++17
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    HelpFuncs.cpp \
    MyPictureBox_realisation.cpp \
    QtTestSQLConn.cpp \
    TestCalc.cpp \
    connector_to_sql_realisation.cpp \
    d_algorithm_realize.cpp \
    main.cpp \
    mainwindow.cpp \
    navigatorhandler_spesialisation.cpp \
    route_calculator.cpp

HEADERS += \
    HelpFuncs.h \
    MyPictureBox_declaration.h \
    TestRoutCalc.h \
    TestsSQLcon.h \
    connector_to_sql_declaration.h \
    d_algorithm_h.h \
    mainwindow.h \
    navigatorhandler_declaration.h \
    route_Calculator.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    map.png

RESOURCES += \
    images.qrc
