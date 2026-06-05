#include "mainwindow.h"
#include <QApplication>
#include <QWidget>
#include <QDebug>
#include <QGraphicsView>

#include <QKeyEvent>
#include "Bride.h"
#include <QDir>
#include "test.h"
#include"Juego.h"



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qDebug() << QDir::currentPath();
    //ventana
    Juego *juego = new Juego();
    QGraphicsView* ventana=new QGraphicsView(juego->getEscena());
    ventana->resize(1200,630);
    ventana->installEventFilter(juego);
    ventana->setWindowTitle("Kill Bill Kendo");
    //fin ventana

    ventana->show();
    return a.exec();
}
