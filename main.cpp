//Codigo de compilacion en la terminal:
//-------------------------------------------------------------------------------------------------------
//qmake
//make
//.CEPac-Man
//-------------------------------------------------------------------------------------------------------

//Comandos de descargas de bibliotecas para Ubuntu:
//-------------------------------------------------------------------------------------------------------
//sudo apt-get update
//sudo apt-get install libsfml-dev
//sudo apt-get update
//sudo apt-get install qt5-default
//-------------------------------------------------------------------------------------------------------

//Configuracion del archivo "CEPac-Man.pro"
//-------------------------------------------------------------------------------------------------------
/*
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
        Pacman.cpp \
        Ghost.cpp

HEADERS += \
        Game.h \
        Nodo.h \
        Nivel.h \
        Pacman.h \
        Ghost.h

FORMS +=

# Añade la ruta a los archivos de cabecera de SFML
INCLUDEPATH += /usr/include/SFML

# Añade la ruta a las bibliotecas de SFML
LIBS += -L/usr/lib/x86_64-linux-gnu -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lsfml-network

# Vincula las bibliotecas de SFML necesarias
LIBS += -lsfml-audio -lsfml-graphics -lsfml-window -lsfml-system
*/
//-------------------------------------------------------------------------------------------------------

//Configuracion del archivo ".vscode/c_cpp_properties.json"
//-------------------------------------------------------------------------------------------------------
/*
{
    "configurations": [
        {
            "name": "Linux",
            "includePath": [
                "/usr/include/x86_64-linux-gnu/qt5",
                "/usr/include/x86_64-linux-gnu/qt5/QtCore",
                "/usr/include/x86_64-linux-gnu/qt5/QtGui",
                "/usr/include/x86_64-linux-gnu/qt5/QtWidgets",
                "/**"
            ],
            "defines": [],
            "compilerPath": "/usr/bin/clang-14",
            "cStandard": "c17",
            "cppStandard": "c++14",
            "intelliSenseMode": "linux-clang-x64",
            "configurationProvider": "ms-vscode.cmake-tools"
        }
    ],
    "version": 4
}
*/
//-------------------------------------------------------------------------------------------------------

//Configuracion del archivo ".vscode/settings.json"
//-------------------------------------------------------------------------------------------------------
/*
{
    "files.associations": {
        "qpushbutton": "cpp",
        "qtimer": "cpp",
        "array": "cpp",
        "chrono": "cpp",
        "compare": "cpp",
        "functional": "cpp",
        "ratio": "cpp",
        "tuple": "cpp",
        "type_traits": "cpp",
        "utility": "cpp",
        "variant": "cpp",
        "atomic": "cpp",
        "bit": "cpp",
        "*.tcc": "cpp",
        "cctype": "cpp",
        "clocale": "cpp",
        "cmath": "cpp",
        "concepts": "cpp",
        "condition_variable": "cpp",
        "cstdarg": "cpp",
        "cstddef": "cpp",
        "cstdint": "cpp",
        "cstdio": "cpp",
        "cstdlib": "cpp",
        "ctime": "cpp",
        "cwchar": "cpp",
        "cwctype": "cpp",
        "deque": "cpp",
        "list": "cpp",
        "map": "cpp",
        "string": "cpp",
        "unordered_map": "cpp",
        "vector": "cpp",
        "exception": "cpp",
        "algorithm": "cpp",
        "iterator": "cpp",
        "memory": "cpp",
        "memory_resource": "cpp",
        "numeric": "cpp",
        "random": "cpp",
        "string_view": "cpp",
        "system_error": "cpp",
        "initializer_list": "cpp",
        "iosfwd": "cpp",
        "limits": "cpp",
        "mutex": "cpp",
        "new": "cpp",
        "numbers": "cpp",
        "ostream": "cpp",
        "semaphore": "cpp",
        "stdexcept": "cpp",
        "stop_token": "cpp",
        "streambuf": "cpp",
        "thread": "cpp",
        "typeinfo": "cpp",
        "qgraphicsscene": "cpp",
        "qgraphicsrectitem": "cpp",
        "set": "cpp",
        "unordered_set": "cpp"
    }
}
*/
//-------------------------------------------------------------------------------------------------------

//En caso del error molesto que no hace nada en realidad, poner este comando:
//-------------------------------------------------------------------------------------------------------
//export QT_QPA_PLATFORM=xcb

#include <QApplication>
#include <QDesktopWidget>
#include "Game.h"

int main(int argc, char *argv[]) {

    QApplication app(argc, argv);
    // Crear la ventana del juego
    Game* gameWindow = new Game();

    // Centrar la ventana en la pantalla
    QDesktopWidget desktop;
    int screenWidth = desktop.screen()->width();
    int screenHeight = desktop.screen()->height();
    int windowWidth = gameWindow->width();
    int windowHeight = gameWindow->height();
    gameWindow->setGeometry((screenWidth - windowWidth) / 2, (screenHeight - windowHeight) / 2, windowWidth, windowHeight);

    //Inicializa la ventaja del juego...
    gameWindow->show();
    return app.exec();
}