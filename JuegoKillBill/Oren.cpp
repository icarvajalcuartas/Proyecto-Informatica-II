#include "Oren.h"
#include "QRandomGenerator"
#include <QGraphicsScene>
#include "Bride.h"
#include <QPainter>

Oren::Oren(float posx, float posy): Personaje("O-Ren", posx, posy, 5), distanciaAmenaza(200.0f),
    distanciaRetroceder(80.0f),
    velocidad(120.0f),
    acumTiempo(0.0f),acumSprite(0.0f),accionActual(Accion::Esperar),modoDebilitado (false),cooldownAtaque(false),tiempoCooldown(0.0f)
{
    seccionarSpritesheet(":/sprites/o-ren.png");
    actualizarSprite();
}
QRectF Oren::boundingRect() const
{
    return QRectF(0, 0,128,128);
}
void Oren::paint(QPainter *painter,
                 const QStyleOptionGraphicsItem *option,
                 QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    painter->drawPixmap(0, 0,spriteActual);
}
void Oren::actualizar(float dt)
{
    acumTiempo += dt;

    if(cooldownAtaque)
    {
        tiempoCooldown += dt;
        if(tiempoCooldown > 1.0f)
        {
            cooldownAtaque = false;
            tiempoCooldown = 0;
        }
    }
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
    actualizarMovimiento(dt);
    actualizarAnimacion(dt);
    actualizarSprite();
}
void Oren::decidirAccion(Personaje* jugador)
{
    Bride* b = dynamic_cast<Bride*>(jugador);
    if(!b) return;

    float dx = b->getPosx() - posx;
    float distancia = std::abs(dx);

    bool brideAtacando = (b->getEstado() == Estado::Atacando);
    dirActual = (dx > 0) ? Direccion::Derecha : Direccion::Izquierda;
    if(cooldownAtaque)
    {
        accionActual = Accion::Esperar;
        zonaActual = ZonaAtaque::Inicial;
        estado = Estado::Quieto;
        velx = 0;
        return;
    }
    if(brideAtacando && distancia < 140)
    {
        velx = (dx > 0 ? -velocidad : velocidad);
        estado = Estado::Moviendose;
        return;
    }

    if(distancia < distanciaAmenaza)
    {
        velx = 0;
        int limite = modoDebilitado ? 9 : 3;
        int r = QRandomGenerator::global()->bounded(limite);

        if(r == 0) iniciarAtaque(ZonaAtaque::Men);
        else if(r == 1) iniciarAtaque(ZonaAtaque::Do);
        else if(r == 2) iniciarAtaque(ZonaAtaque::Kote);
        cooldownAtaque = true;
        tiempoCooldown = 0;

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

void Oren::recibirGolpe()
{
    vida--;

    estado = Estado::Golpeado;
    frameActual = 0;

    actualizarSprite();
}

void Oren::actualizarMovimiento(float dt)
{
    posx += velx * dt;
    setPos(posx, posy);
}

void Oren::actualizarAnimacion(float dt)
{
    acumSprite += dt;

    if(acumSprite < 0.15f)
        return;

    acumSprite = 0;

    QVector<QPixmap>* frames = obtenerSpriteActual();

    if(!frames || frames->isEmpty())
        return;

    frameActual = (frameActual + 1) % frames->size();
}

void Oren::activarModoDebilitado()
{
    modoDebilitado = true;
    velocidad = 60.0f;
}

void Oren::desactivarModoDebilitado()
{
    modoDebilitado = false;
    velocidad = 120.0f;
}

