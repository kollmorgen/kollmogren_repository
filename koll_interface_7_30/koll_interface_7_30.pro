#-------------------------------------------------
#
# Project created by QtCreator 2019-05-30T15:16:07
#
#-------------------------------------------------

QT       += core gui
QT       += charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = koll_interface_7_30
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        Fsensor.cpp \
        koll_plot.cpp \
        koll_queue.cpp \
        koll_tcp.cpp \
        main.cpp \
        koll_interface_7_30.cpp

HEADERS += \
        Fsensor.h \
        koll_interface_7_30.h \
        koll_plot.h \
        koll_queue.h \
        koll_tcp.h

FORMS += \
        koll_interface_7_30.ui

RESOURCES += \
    image.qrc
