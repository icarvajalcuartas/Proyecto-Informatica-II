#include "Oren.h"
#include "QRandomGenerator"

Oren::Oren() {}

Oren::Oren(float posx, float posy): Personaje("O-Ren", posx, posy, 5),
    distanciaAmenaza(200.0f),
    distanciaRetroceder(80.0f),
    velocidad(120.0f),
    acumTiempo(0),
{
    seccionarSpritesheet(":/sprites/o-ren.png");
    actualizarSprite();
}
QRectF Oren::boundingRect() const
{
    return QRectF(0,0,128,128);
}
void Oren::paint(QPainter *painter,
                 const QStyleOptionGraphicsItem *option,
                 QWidget *widget)
{
    painter->drawPixmap(0,0,spriteActual);
    painter->drawRect(getZonaMen());
    painter->drawRect(getZonaDo());
    painter->drawRect(getZonaKote());
}
void Oren::actualizar(float dt)
{
    acumTiempo += dt;

    if(acumTiempo >= 0.2f)
    {
        acumTiempo = 0;

        if(scene())
        {
            for(QGraphicsItem* item : scene()->items())
            {
                Bride* b = dynamic_cast<Bride*>(item);
                if(b)
                {
                    decidirAccion(b);
                    break;
                }
            }
        }
    }

    actualizarSprite();
}
void Oren::decidirAccion(Personaje* jugador)
{
    float dx = jugador->getPosx() - this->posx;
    float distancia = std::abs(dx);

    dirActual = (dx > 0)? Direccion::Derecha: Direccion::Izquierda;

    if(distancia < distanciaRetroceder)
    {
        velx = (dx > 0 ? -velocidad : velocidad);
        estado = Estado::Moviendose;
        return;
    }

    if(distancia < distanciaAmenaza)
    {
        velx = 0;

        int r = QRandomGenerator::global()->bounded(3);

        if(r == 0) iniciarAtaque(ZonaAtaque::Men);
        else if(r == 1) iniciarAtaque(ZonaAtaque::Do);
        else iniciarAtaque(ZonaAtaque::Kote);

        return;
    }

    velx = (dx > 0 ? velocidad : -velocidad);
    estado = Estado::Moviendose;
}
void Oren::iniciarAtaque(ZonaAtaque zona)
{
    if(estado == Estado::Atacando)
        return;

    zonaActual = zona;
    estado = Estado::Atacando;
    frameActual = 0;

    actualizarSprite();
}
QRectF Oren::getHitboxAtaque() const
{
    if(estado != Estado::Atacando)
        return QRectF();

    switch(zonaActual)
    {
    case ZonaAtaque::Men:
        return mapRectToScene(QRectF(0,0,80,40));
    case ZonaAtaque::Do:
        return mapRectToScene(QRectF(0,40,80,30));
    case ZonaAtaque::Kote:
        return mapRectToScene(QRectF(0,75,80,25));
    default:
        return QRectF();
    }
}
QRectF Oren::getHitboxCuerpo() const
{
    return mapRectToScene(QRectF(20,5,70,100));
}

