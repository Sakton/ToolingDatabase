#-------------------------------------------------
#
# Project created by QtCreator 2016-11-16T13:40:21
#
#-------------------------------------------------

QT += core gui sql printsupport core5compat

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DB_OTPI_SDI
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    finddialog.cpp \
    mysqltablemodel.cpp \
    myform.cpp

HEADERS  += mainwindow.h \
    finddialog.h \
    mysqltablemodel.h \
    myform.h

FORMS    += mainwindow.ui \
    finddialog.ui \
    myform.ui

RESOURCES += \
    resource.qrc

DISTFILES += \
    mystyle.css
