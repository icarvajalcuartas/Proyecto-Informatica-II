#include "Bride.h"
#include <QGraphicsObject>
#include <QString>
#include <QPixmap>
#include <QVector>
#include <QMap>
#include <QPainter>

Bride::Bride(float posx, float posy):Personaje("The Bride",posx,posy,5),
    contZanshin(0),puntaje(0),fisicaActual(ModoFisica::Uniforme),
    enSuelo(true),ultimoAtaquevalido(ZonaAtaque::Inicial){
    seccionarSpritesheet(":/sprites/thebride_2.png");
    actualizarSprite();
}

QRectF Bride::boundingRect() const {return QRectF(0,0,128,128);}

void Bride::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    qDebug() << "paint ejecutado";
    qDebug() << "sprite nulo:" << spriteActual.isNull();
    qDebug() << spriteActual.width()
             << spriteActual.height();
    painter->drawRect(0,0,128,128);
    painter->drawPixmap(0,0,spriteActual);
    painter->drawRect(getZonaMen());
    painter->drawRect(getZonaDo());
    painter->drawRect(getZonaKote());
}

void Bride::actualizar(float difTiempo)
{
    if (zanshinEspecialActivo){

        tiempoZanshinEspecial -= difTiempo;
        if(tiempoZanshinEspecial <=0){

            zanshinEspecialActivo = false;
            tiempoZanshinEspecial = 0.0f;
            contZanshin = 0;
            emit zanshinEspecialterminado();
            emit zanshinActualizado(getContZanshin());
        }
    }
    switch(fisicaActual){
    case (ModoFisica::Uniforme):{
        movRectilineo();
        break;
    }
    case (ModoFisica::Parabolica):{
        movParabolico(difTiempo);
        break;
    }
    case (ModoFisica::dos5Dimensiones):{
        movDosDimensiones(difTiempo);
        break;
    }
    case (ModoFisica::SaltoY):{
        movSaltoY(difTiempo);
        break;
    }
    }

    tiempoAnimacion += difTiempo;

    if(tiempoAnimacion >= DURACION_FRAME)
    {
        avanzarFrame();
        tiempoAnimacion -= DURACION_FRAME;
    }
    actualizarSprite();
}
QRectF Bride::getHitboxAtaque() const
{
    if(estado != Estado::Atacando)
        return QRectF();

    switch(zonaActual)
    {
    case ZonaAtaque::Men:
        return mapRectToScene(QRectF(90,0,80,40));

    case ZonaAtaque::Do:
        return mapRectToScene(QRectF(90,40,80,30));

    case ZonaAtaque::Kote:
        return mapRectToScene(QRectF(90,75,80,25));

    default:
        return QRectF();
    }
}

void Bride::sumarPunto()
{
    puntaje++;

    if(puntaje>=5){
        emit victoria();
    }
}

void Bride::setModoFisica(ModoFisica modo)
{
    fisicaActual=modo;
}

void Bride::setDireccion(Direccion dir)
{
    dirActual = dir;
    if(dir == Direccion::Izquierda){
        velx = -FUERZA_HORIZONTAL;
    }
    else if(dir == Direccion::Derecha) {
        velx =  FUERZA_HORIZONTAL;
    }
    else{
        velx =  0;
    }
}

void Bride::setEnSuelo(bool suelo)
{
    enSuelo = suelo;
}


void Bride::realizarParry()
{
    estado=Estado::Defendiendo;
}

void Bride::recibirGolpe()
{
    contZanshin = 0;
    zanshinEspecialActivo = false;
    emit zanshinEspecialterminado();
    emit zanshinActualizado(getContZanshin());
}

void Bride::input()
{

}


void Bride::iniciarAtaque(ZonaAtaque zona)
{
    zonaActual=zona;
    estado=Estado::Atacando;
}

void Bride::registrarZanshin(ZonaAtaque zona)
{
    if(ultimoAtaquevalido==zona){

        contZanshin=0;
    }else{

        contZanshin++;

        if (contZanshin >= 3 && !zanshinEspecialActivo){
            zanshinEspecialActivo = true;
            tiempoZanshinEspecial = DUR_ZANSHINESPECIAL;

            emit zanshinEspecialIniciado();
            emit zanshinActualizado(getContZanshin());

        }
    }
    ultimoAtaquevalido = zona;
}

void Bride::movRectilineo()
{
    posx += velx;
    setPos(posx, posy);
}


void Bride::movSaltoY(float difTiempo)
{/*
    vely += 2 * difTiempo;
    posy += vely  * difTiempo;
    setPos(posx, posy);*/
    vely += 500.0f * difTiempo;
    posy += vely * difTiempo;

    if(posy >= 300.0f)
    {
        posy = 300.0f;
        vely = 0;

        enSuelo = true;

        estado = Estado::Quieto;

        fisicaActual = ModoFisica::Uniforme;
        frameActual = 0;
    }

    setPos(posx,posy);
}

void Bride::movParabolico(float difTiempo)
{
    vely += acely * difTiempo;
    posx += velx * difTiempo;
    posy += vely * difTiempo;
    setPos(posx, posy);
}

void Bride::movDosDimensiones(float difTiempo)
{
    velx += acelx * difTiempo;
    vely += acely * difTiempo;
    posx += velx * difTiempo;
    posy += vely * difTiempo;
    setPos(posx, posy);
}


bool Bride::getEnSuelo() const
{
    return enSuelo;
}

unsigned short Bride::getContZanshin() const
{
    return contZanshin;
}

void Bride::saltar()
{
    if(enSuelo){
        vely = -FUERZA_SALTO;
        velx=0;
        estado = Estado::Saltando;
        fisicaActual = ModoFisica::SaltoY;
        enSuelo=false;
    }
}

void Bride::moverDerecha()
{
    estado = Estado::Moviendose;
    dirActual = Direccion::Derecha;
    fisicaActual = ModoFisica::Uniforme;
    velx= FUERZA_HORIZONTAL;
}

void Bride::moverIzquierda()
{
    estado = Estado::Moviendose;
    dirActual = Direccion::Izquierda;
    fisicaActual = ModoFisica::Uniforme;
    velx= -FUERZA_HORIZONTAL;
}

void Bride::moverAtras()
{

}

void Bride::moverAdelante()
{

}

void Bride::saltarDerecha()
{
    if(!zanshinEspecialActivo || !enSuelo)
        return;

    estado = Estado::Saltando;

    fisicaActual = ModoFisica::Parabolica;

    velx = 200.0f;
    vely = -FUERZA_SALTO;

    enSuelo = false;
}

void Bride::saltarIzquierda()
{
    if(!zanshinEspecialActivo || !enSuelo)
        return;

    estado = Estado::Saltando;

    fisicaActual = ModoFisica::Parabolica;

    velx = -200.0f;
    vely = -FUERZA_SALTO;

    enSuelo = false;
}

void Bride::detenerMovimiento()
{
    estado= Estado::Quieto;
    zonaActual= ZonaAtaque::Inicial;
    velx=0;
    frameActual=0;
    actualizarSprite();
}

void Bride::ataqueMen()
{
    estado = Estado::Atacando;
    zonaActual = ZonaAtaque::Men;
}

void Bride::ataqueDo()
{
    estado = Estado::Atacando;
    zonaActual = ZonaAtaque::Do;
}

void Bride::ataqueKote()
{
    estado = Estado::Atacando;
    zonaActual = ZonaAtaque::Kote;
}


