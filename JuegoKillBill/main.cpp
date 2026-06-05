#include "mainwindow.h"
#include <QApplication>
#include <QWidget>
#include <QDebug>
#include <QGraphicsView>

#include <QKeyEvent>
#include "Bride.h"
#include <QDir>
#include "test.h"



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qDebug() << QDir::currentPath();
    //ventana
    QGraphicsView* ventana;

    //fin ventana
    //scene y poner a bride
    QGraphicsScene* scene = new QGraphicsScene();
    ventana = new QGraphicsView(scene);
    ventana->setWindowTitle("Kill Bill Kendo");
    ventana->resize(1200,630);

    Bride* bride = new Bride(100,100);

    scene->addItem(bride);
    bride->setPos(100,100);

    //prueba de perifericos
    Test* input = new Test(bride);
    ventana->installEventFilter(input);
    // fin prueba perifericos

    ventana->show();
    return a.exec();
}
