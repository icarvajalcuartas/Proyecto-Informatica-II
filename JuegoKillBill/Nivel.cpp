#include "Nivel.h"

Nivel::Nivel(unsigned short int numero, QGraphicsScene *escena): numeroNivel(numero),
    escena(escena), bride(nullptr), terminado(false){}

void Nivel::cargar()
{
    bride = new Bride(100,300);
    escena->addItem(bride);
    //bride->setPos(100,100);
}

void Nivel::inputJugador(QKeyEvent *evento)
{
    if(!bride)
        return;

    switch(evento->key())
    {
    case Qt::Key_A:
        bride->moverIzquierda();
        break;

    case Qt::Key_D:
        bride->moverDerecha();
        break;

    case Qt::Key_Space:
        bride->saltar();
        break;

    case Qt::Key_J:
        bride->ataqueMen();
        break;

    case Qt::Key_K:
        bride->ataqueDo();
        break;

    case Qt::Key_L:
        bride->ataqueKote();
        break;
    }
}

void Nivel::inputJugadorLiberada(QKeyEvent *evento)
{
    if(!bride)
        return;

    switch(evento->key())
    {
    case Qt::Key_A:
    case Qt::Key_D:
        bride->detenerMovimiento();
        break;
    }
}

void Nivel::actualizar(float dt)
{
    if (bride){
        bride->actualizar(dt);
    }
}
