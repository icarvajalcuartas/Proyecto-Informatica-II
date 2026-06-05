#ifndef NIVEL_H
#define NIVEL_H
#include "Bride.h"
#include "Oren.h"
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
    Oren* oren;
    unsigned short puntosBride = 0;
    unsigned short puntosOren = 0;
    QString rutaFondo;
    bool terminado;

    const unsigned short MAX_PUNTOS = 5;
public:

    Nivel(unsigned short int numero, QGraphicsScene* escena);
    void cargar();
    void inputJugador(QKeyEvent* evento);
    void inputJugadorLiberada(QKeyEvent *evento);
    void actualizar(float dt);
    void verificarColisiones();
    bool getTerminado() const;
    unsigned short int getNumeroNivel() const;
    Bride* getBride() const;
signals:
    void derrota();
    void victoria();
};

#endif // NIVEL_H
