#include "Nivel.h"

Nivel::Nivel(unsigned short int numero, QGraphicsScene *escena): numeroNivel(numero),
    escena(escena), bride(nullptr), terminado(false){}

void Nivel::cargar()
{
    bride = new Bride(600,300);
    oren = new Oren(300,300);
    escena->addItem(bride);
    escena->addItem(oren);
    connect(bride, &Bride::zanshinActualizado,this, [](unsigned short c){
                qDebug() << "Zanshin:" << c;
    });

    connect(bride, &Bride::zanshinEspecialIniciado,
            this, [](){qDebug() << "Zanshin especial ACTIVADO";
            });

    connect(bride, &Bride::zanshinEspecialterminado,
            this, [](){
                qDebug() << "Zanshin terminado";
            });

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

    if(bride) bride->actualizar(dt);
    if(oren) oren->actualizar(dt);

    verificarColisiones();

}
void Nivel::verificarColisiones()
{
    if(!bride || !oren)
        return;
    if(!bride || !oren)
        return;

    QRectF ataqueBride = bride->getHitboxAtaque();
    QRectF bodyOren = oren->getHitboxCuerpo();

    QRectF ataqueOren = oren->getHitboxAtaque();
    QRectF bodyBride = bride->getHitboxCuerpo();


    if(ataqueBride.intersects(bodyOren))
    {
        puntosBride++;
        oren->recibirGolpe();

        bride->registrarZanshin(oren->getZonaActual());
    }


    if(ataqueOren.intersects(bodyBride))
    {
        puntosOren++;
        bride->recibirGolpe();
    }

    if(puntosBride >= MAX_PUNTOS)
    {
        emit victoria();
        return;
    }

    if(puntosOren >= MAX_PUNTOS)
    {
        emit derrota();
        return;
    }
}
