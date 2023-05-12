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
        Nivel.cpp \
        Pacman.cpp

HEADERS += \
        Game.h \
        Nodo.h \
        Nivel.h \
        Pacman.h

FORMS +=

# Añade la ruta a los archivos de cabecera de SFML
INCLUDEPATH += /usr/include/SFML

# Añade la ruta a las bibliotecas de SFML
LIBS += -L/usr/lib/x86_64-linux-gnu -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lsfml-network

# Vincula las bibliotecas de SFML necesarias
LIBS += -lsfml-audio -lsfml-graphics -lsfml-window -lsfml-system