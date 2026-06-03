#include "Bride.h"
#include <QGraphicsObject>
#include <QString>
#include <QPixmap>
#include <QVector>
#include <QMap>
#include <QPainter>

Bride::Bride(float posx, float posy):Personaje("The Bride",posx,posy,5),
    contZanshin(0),puntaje(0),ataqueValido(false),fisicaActual(ModoFisica::Uniforme),
    enSuelo(true){

    dirActual = Direccion::Adelante;
    estado = Estado::Quieto;
    cargarSprites(":/sprites/thebride_2.png",estado,dirActual,
                  ZonaAtaque::Inicial,4, 2, 128, 128);
    actualizarSprite();
}

QRectF Bride::boundingRect() const
{
    return QRectF(0,0,128,128);
}

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

QRectF Bride::getHitboxAtaque() const
{
    if(estado != Estado::Atacando)
        return QRectF();

    switch(zonaActual)
    {
    case ZonaAtaque::Men:
        return QRectF(90,0,80,40);

    case ZonaAtaque::Do:
        return QRectF(90,40,80,30);

    case ZonaAtaque::Kote:
        return QRectF(90,75,80,25);

    default:
        return QRectF();
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

void Bride::input()
{

}

void Bride::actualizar(float difTiempo)
{
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

    tiempoAnimacion -= DURACION_FRAME;

    if(tiempoAnimacion >= DURACION_FRAME)
    {
        avanzarFrame();
        tiempoAnimacion -= DURACION_FRAME;
    }
    actualizarSprite();
}

void Bride::iniciarAtaque(ZonaAtaque zona)
{
    zonaActual=zona;
    estado=Estado::Atacando;
}

void Bride::esAtaqueValido(ZonaAtaque zona)
{
    if(zonaActual==zona){
        ataqueValido=false;
        contZanshin=0;
    }else{
        ataqueValido=true;
        contZanshin+=1;
    }
}

void Bride::movRectilineo()
{
    posx += velx;
    setPos(posx, posy);
}


void Bride::movSaltoY(float difTiempo)
{
    vely += acely * difTiempo;
    posy += vely  * difTiempo;
    setPos(posx, posy);
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

void Bride::detenerMovimiento()
{
    estado= Estado::Quieto;
    zonaActual= ZonaAtaque::Inicial;
    velx=0;
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


