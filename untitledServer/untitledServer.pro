QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += "C:\Users\hobbi\Downloads\boost_1_78_0_rc1\boost_1_78_0"

SOURCES += \
    imageinfo.cpp \
    main.cpp \
    mainwindow.cpp \
    messagestream.cpp \
    net_msg.cpp \
    postmanager.cpp \
    roominfo.cpp \
    server.cpp \
    userinfo.cpp

HEADERS += \
    imageinfo.h \
    mainwindow.h \
    messagestream.h \
    net_msg.h \
    postmanager.h \
    roominfo.h \
    server.h \
    userinfo.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
