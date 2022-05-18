QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += "C:\Users\hobbi\Downloads\boost_1_78_0_rc1\boost_1_78_0"

SOURCES += \
    alluserswidget.cpp \
    buttondelegate.cpp \
    client.cpp \
    createroomdialog.cpp \
    friendsmodel.cpp \
    friendswidget.cpp \
    imageinfo.cpp \
    logindialog.cpp \
    main.cpp \
    mainwindow.cpp \
    messagesmodel.cpp \
    messagestream.cpp \
    net_msg.cpp \
    plugdelegate.cpp \
    postrecipient.cpp \
    profilewidget.cpp \
    registrationdialog.cpp \
    roominfo.cpp \
    roomslist.cpp \
    roomwidget.cpp \
    settingswidget.cpp \
    userinfo.cpp

HEADERS += \
    alluserswidget.h \
    buttondelegate.h \
    client.h \
    createroomdialog.h \
    friendsmodel.h \
    friendswidget.h \
    imageinfo.h \
    logindialog.h \
    mainwindow.h \
    messagesmodel.h \
    messagestream.h \
    net_msg.h \
    plugdelegate.h \
    postrecipient.h \
    profilewidget.h \
    registrationdialog.h \
    roominfo.h \
    roomslist.h \
    roomwidget.h \
    settingswidget.h \
    userinfo.h

FORMS += \
    alluserswidget.ui \
    createroomdialog.ui \
    friendswidget.ui \
    logindialog.ui \
    mainwindow.ui \
    profilewidget.ui \
    registrationdialog.ui \
    roomslist.ui \
    roomwidget.ui \
    settingswidget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
