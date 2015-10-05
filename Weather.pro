#-------------------------------------------------
#
# Project created by QtCreator 2015-09-24T14:54:40
#
#-------------------------------------------------

QT       += core gui sql network xml

CONFIG += C++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Weather
TEMPLATE = app

INCLUDEPATH += $$PWD
INCLUDEPATH += ../impl


SOURCES += main.cpp\
        mainwindow.cpp \
    commonsettingsdialog.cpp \
    databasehandler.cpp \
    impl/yandexweaterproducer.cpp \
    yandexweatersettingdialog.cpp \
    weaterhandler.cpp \
    impl/yandexweathercreator.cpp \
    impl/openweathermapproducer.cpp \
    impl/openweathermapcreator.cpp \
    openweathermapsittingdialog.cpp

HEADERS  += mainwindow.h \
    commonsettingsdialog.h \
    databasehandler.h \
    interfaces/iweaterinterface.h \
    interfaces/igetweater.h \
    impl/yandexweaterproducer.h \
    yandexweatersettingdialog.h \
    weaterhandler.h \
    impl/yandexweathercreator.h \
    interfaces/iviewweatherinfo.h \
    interfaces/ibuttonclick.h \
    impl/openweathermapproducer.h \
    impl/openweathermapcreator.h \
    openweathermapsittingdialog.h

FORMS    += mainwindow.ui \
    commonsettingsdialog.ui \
    yandexweatersettingdialog.ui \
    openweathermapsittingdialog.ui
