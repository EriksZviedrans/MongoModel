#-------------------------------------------------
#
# Project created by QtCreator 2015-05-25T19:29:47
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MongoModel
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    mongolistmodel.cpp \
    mongoitems.cpp

HEADERS  += mainwindow.h \
    mongolistmodel.h \
    mongoitems.h

FORMS    += mainwindow.ui

unix:!macx: LIBS += -lmongoclient
unix:!macx: LIBS += -pthread
unix:!macx: LIBS += -lboost_thread
unix:!macx: LIBS += -lboost_system
unix:!macx: LIBS += -lboost_filesystem
unix:!macx: LIBS += -lboost_program_options
unix:!macx: LIBS += -lboost_regex
