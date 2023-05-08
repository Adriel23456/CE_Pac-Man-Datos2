TEMPLATE = app
TARGET = CEPac-Man
INCLUDEPATH += .

QT += core gui widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

SOURCES += \
        main.cpp \
        Game.cpp \
        Nodo.cpp \
        Nivel.cpp 

HEADERS += \
        Game.h \
        Nodo.h \
        Nivel.h 

FORMS +=