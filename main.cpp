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
#include <QDesktopWidget>
#include "Game.h"
#include "Nivel.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    // Crear la ventana del juego
    Game gameWindow;

    // Centrar la ventana en la pantalla
    QDesktopWidget desktop;
    int screenWidth = desktop.screen()->width();
    int screenHeight = desktop.screen()->height();
    int windowWidth = gameWindow.width();
    int windowHeight = gameWindow.height();
    gameWindow.setGeometry((screenWidth - windowWidth) / 2, (screenHeight - windowHeight) / 2, windowWidth, windowHeight);

    //Inicializa la ventaja del juego...
    gameWindow.show();
    return app.exec();
}