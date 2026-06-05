#ifndef NIVEL_H
#define NIVEL_H
#include "Bride.h"
#include <QObject>
#include <QGraphicsScene>
#include <QEvent>
#include <QKeyEvent>

class Nivel:public QObject
{
    Q_OBJECT
private:
    unsigned short int numeroNivel;
    QGraphicsScene* escena;
    Bride* bride;
    QString rutaFondo;
    bool terminado;
public:

    Nivel(unsigned short int numero, QGraphicsScene* escena);
    void cargar();
    void inputJugador(QKeyEvent* evento);
    void actualizar(float dt);
    void verificarColisiones();
    bool getTerminado() const;
    unsigned short int getNumeroNivel() const;
    Bride* getBride() const;
};

#endif // NIVEL_H
