#include "mainwindow.h"
#include <QApplication>
#include <QWidget>
#include <QDebug>
#include "Bride.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //MainWindow w;
    QWidget ventana;
    ventana.setWindowTitle("Kill Bill Kendo");
    ventana.resize(1200, 630);
    ventana.show();
    // Bride jugador(400,340,0);
    // qDebug()<<jugador.getVida();
    //w.show();
    return a.exec();
}
