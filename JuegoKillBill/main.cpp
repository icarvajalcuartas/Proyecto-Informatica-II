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

    Juego *juego = new Juego();
    QGraphicsView* ventana=new QGraphicsView();
    ventana->resize(1200,630);
    ventana->installEventFilter(juego);
    ventana->setWindowTitle("Kill Bill Kendo");
    ventana->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ventana->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    ventana->show();
    juego->iniciarMenu(ventana);

    return a.exec();
}
