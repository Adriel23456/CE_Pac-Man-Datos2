//Codigo de compilacion:
//-------------------------------------------------------------------------------------------------------
//moc -I/usr/include/x86_64-linux-gnu/qt5 mainwindow.h -o moc_mainwindow.cpp
//g++ main.cpp mainwindow.h moc_mainwindow.cpp -fPIC -o CEPac-Man.exe $(pkg-config --cflags --libs Qt5Core Qt5Widgets)
//./CEPac-Man.exe
//-------------------------------------------------------------------------------------------------------
//g++ main.cpp -fPIC -o CEPac-Man.exe $(pkg-config --cflags --libs Qt5Core Qt5Widgets)
//./CEPac-Man.exe
//-------------------------------------------------------------------------------------------------------
//En caso de error...
//export QT_QPA_PLATFORM_PLUGIN_PATH=/usr/lib/x86_64-linux-gnu/qt5/plugins/platforms/
//export QT_QPA_PLATFORM=xcb


#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsView>
#include <QKeyEvent>

class Character : public QGraphicsRectItem {
public:
    void keyPressEvent(QKeyEvent *event) override {
        int moveDistance = 10;
        if (Qt::Key_Left){
            setPos(x() - moveDistance, y());
        }
        if (Qt::Key_Right){
            setPos(x() + moveDistance, y());
        }
        if (Qt::Key_Up){
            setPos(x(), y() - moveDistance);
        }
        if (Qt::Key_Down){
            setPos(x(), y() + moveDistance);
        }
    }
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Crea una escena
    QGraphicsScene scene;

    // Crea un personaje y lo añade a la escena
    Character *character = new Character();
    character->setRect(0, 0, 50, 50);
    scene.addItem(character);

    // Habilita el manejo de eventos de teclado para el personaje
    character->setFlag(QGraphicsItem::ItemIsFocusable);
    character->setFocus();

    // Crea una vista para visualizar la escena
    QGraphicsView view(&scene);
    view.setRenderHint(QPainter::Antialiasing);
    view.setFixedSize(800, 600);
    view.setSceneRect(0, 0, 800, 600);

    // Muestra la vista
    view.show();

    return app.exec();
}