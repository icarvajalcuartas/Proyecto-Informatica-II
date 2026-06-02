#include "mainwindow.h"
#include <QApplication>
#include <QWidget>
#include <QDebug>
#include <QGraphicsView>
#include "Bride.h"
#include <QDir>



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //MainWindow w;
    // QGraphicsView * ventana;
    // ventana.setWindowTitle("Kill Bill Kendo");
    // ventana.resize(1200, 630);
    // ventana.show();
    qDebug() << QDir::currentPath();
    QGraphicsScene* scene = new QGraphicsScene();

    Bride* bride = new Bride(100,100);

    scene->addItem(bride);
    bride->setPos(100,100);

    QGraphicsView* ventana = new QGraphicsView(scene);
    ventana->setWindowTitle("Kill Bill Kendo");
    ventana->resize(1200,630);
    ventana->show();
    // Bride jugador(400,340,0);
    // qDebug()<<jugador.getVida();
    //w.show();
    return a.exec();
}
