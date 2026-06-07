#include "mainwindow.h"
#include <QApplication>
#include <QWidget>
#include <QDebug>
#include <QGraphicsView>
#include <QKeyEvent>
#include"Juego.h"



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    Juego *juego = new Juego();
    QGraphicsView* ventana=new QGraphicsView();
    ventana->resize(1200,630);
    ventana->setWindowTitle("Kill Bill Kendo");
    ventana->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ventana->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ventana->installEventFilter(juego);

    ventana->setScene(nullptr);
    ventana->setRenderHint(QPainter::Antialiasing);
    ventana->setSceneRect(0, 0, 1200, 630);
    ventana->fitInView(0, 0, 1200, 630, Qt::IgnoreAspectRatio);

    juego->iniciarMenu(ventana);
    ventana->show();


    return a.exec();
}
