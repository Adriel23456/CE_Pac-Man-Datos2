//Codigo de compilacion:
//-------------------------------------------------------------------------------------------------------
//qmake
//make
//SOURCES += main.cpp Game.cpp
//HEADERS += Game.h
//-------------------------------------------------------------------------------------------------------
//En caso de error...
//export QT_QPA_PLATFORM=xcb


#include <QApplication>
#include "Game.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    // Crear e iniciar la ventana del juego
    Game gameWindow;
    gameWindow.show();
    return app.exec();
}