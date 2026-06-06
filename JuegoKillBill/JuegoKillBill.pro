QT       += core gui
QT += multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Bride.cpp \
    Juego.cpp \
    Nivel.cpp \
    Oren.cpp \
    Personaje.cpp \
    main.cpp \
    mainwindow.cpp \
    test.cpp

HEADERS += \
    Bride.h \
    Juego.h \
    Nivel.h \
    Oren.h \
    Personaje.h \
    mainwindow.h \
    test.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    thebride.png

RESOURCES += \
    sprites.qrc
