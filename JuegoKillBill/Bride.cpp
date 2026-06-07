#include "Bride.h"
#include <QGraphicsObject>
#include <QString>
#include <QPixmap>
#include <QVector>
#include <QMap>
#include <QPainter>

Bride::Bride(float posx, float posy):Personaje("The Bride",posx,posy,5),
    contZanshin(0),zanshinEspecialActivo(false),tiempoZanshinEspecial (0.0f),
    fisicaActual(ModoFisica::Uniforme),enSuelo(true),tiempoGolpe (0.0f),
    tiempoAnimacion (0.0f),ultimoAtaquevalido(ZonaAtaque::Inicial),
    enModoParry (false),tiempoParryActivo (0.0f),zonaParryActual(ZonaAtaque::Inicial)
{
    seccionarSpritesheet(":/sprites/thebride_2.png");
    actualizarSprite();
}

QRectF Bride::boundingRect() const {return QRectF(0, 0,128,128);}

void Bride::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    painter->drawPixmap(0, 0,spriteActual);
}

void Bride::actualizar(float difTiempo)
{
    actualizarZanshin(difTiempo);
    actualizarParry(difTiempo);
    actualizarMovimiento(difTiempo);
    actualizarAnimacion(difTiempo);
    actualizarSprite();
}

void Bride::actualizarZanshin(float dt)
{
    if(!zanshinEspecialActivo)
        return;

    tiempoZanshinEspecial -= dt;

    if(tiempoZanshinEspecial <= 0)
    {
        zanshinEspecialActivo = false;
        tiempoZanshinEspecial = 0.0f;
        contZanshin = 0;

        emit zanshinEspecialterminado();
        emit zanshinActualizado(contZanshin);
    }
}


void Bride::actualizarMovimiento(float dt)
{
    switch(fisicaActual)
    {
    case ModoFisica::Uniforme:
        movRectilineo(dt);
        break;

    case ModoFisica::Parabolica:
        movParabolico(dt);
        break;

    case ModoFisica::dos5Dimensiones:
        movDosDimensiones(dt);
        break;

    case ModoFisica::SaltoY:
        movSaltoY(dt);
        break;
    }
}

void Bride::actualizarAnimacion(float dt)
{
    tiempoAnimacion += dt;

    if(tiempoAnimacion < DURACION_FRAME)
        return;

    tiempoAnimacion -= DURACION_FRAME;

    QVector<QPixmap>* frames = obtenerSpriteActual();

    if(!frames || frames->isEmpty())
        return;

    if(estado == Estado::Atacando)
    {
        frameActual++;

        if(frameActual >= frames->size())
        {
            frameActual = 0;
            zonaActual = ZonaAtaque::Inicial;

            if(velx != 0)
                estado = Estado::Moviendose;
            else
                estado = Estado::Quieto;
        }
    }
    else if(estado == Estado::Golpeado)
    {
        tiempoGolpe += DURACION_FRAME;
        if(tiempoGolpe >= DURACION_GOLPE)
        {
            estado = Estado::Quieto;
            frameActual = 0;
            tiempoAnimacion = 0;
            tiempoGolpe = 0;
        }
        avanzarFrame();
    }
    else
    {
        avanzarFrame();
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

void Bride::recibirGolpe()
{
    contZanshin = 0;
    zanshinEspecialActivo = false;
    estado = Estado::Golpeado;
    frameActual = 0;
    tiempoGolpe = 0;
    emit zanshinEspecialterminado();
    emit zanshinActualizado(getContZanshin());
}

void Bride::iniciarAtaque(ZonaAtaque zona)
{
    if(estado == Estado::Atacando)
        return;
    zonaActual=zona;
    estado=Estado::Atacando;

    frameActual = 0;
    actualizarSprite();
}

void Bride::ataqueMen()
{
    iniciarAtaque(ZonaAtaque::Men);
}

void Bride::ataqueDo()
{
    iniciarAtaque(ZonaAtaque::Do);
}

void Bride::ataqueKote()
{
    iniciarAtaque(ZonaAtaque::Kote);
}

void Bride::registrarZanshin(ZonaAtaque zona)
{
    if(ultimoAtaquevalido==zona){

        contZanshin=0;
    }
    else{

        contZanshin++;
    }
    ultimoAtaquevalido = zona;
    emit zanshinActualizado(getContZanshin());

    if (contZanshin >= 3 && !zanshinEspecialActivo){
        zanshinEspecialActivo = true;
        tiempoZanshinEspecial = DUR_ZANSHINESPECIAL;

        emit zanshinEspecialIniciado();
    }
}

void Bride::falloAtaque()
{
    contZanshin = 0;
    ultimoAtaquevalido = ZonaAtaque::Inicial;
    emit zanshinActualizado(contZanshin);
}

void Bride::movRectilineo(float difTiempo)
{
    posx += velx * difTiempo;
    setPos(posx, posy);
}

void Bride::movSaltoY(float difTiempo)
{
    vely += 500.0f * difTiempo;
    posy += vely * difTiempo;

    if(posy >= SUELO_Y)
    {
        posy = SUELO_Y;
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
    if(posy >= SUELO_Y)
    {
        posy    = SUELO_Y;
        vely = 0;
        velx = 0;
        acely = 0;
        enSuelo = true;
        estado  = Estado::Quieto;
        fisicaActual = ModoFisica::Uniforme;
        frameActual  = 0;
    }
    setPos(posx, posy);
}

void Bride::movDosDimensiones(float difTiempo)
{
    velx += acelx * difTiempo;
    vely += acely * difTiempo;
    velx = std::max(-VEL_MAX, std::min(VEL_MAX, velx));
    vely = std::max(-VEL_MAX, std::min(VEL_MAX, vely));

    if(acelx == 0.0f)
    {
        if(velx > 0) velx = std::max(0.0f, velx - FRICCION * difTiempo);
        else         velx = std::min(0.0f, velx + FRICCION * difTiempo);
    }
    if(acely == 0.0f)
    {
        if(vely > 0) vely = std::max(0.0f, vely - FRICCION * difTiempo);
        else         vely = std::min(0.0f, vely + FRICCION * difTiempo);
    }
    posx += velx * difTiempo;
    posy += vely * difTiempo;
    setPos(posx, posy);
}

void Bride::actualizarParry(float dt)
{
    if(!enModoParry) return;

    tiempoParryActivo += dt;
    if(tiempoParryActivo >= DUR_PARRY)
        desactivarParry();
}

bool Bride::getEnSuelo() const
{
    return enSuelo;
}

bool Bride::getZanshinEspecialActivo() const
{
    return zanshinEspecialActivo;
}

unsigned short Bride::getContZanshin() const
{
    return contZanshin;
}

bool       Bride::getEnModoParry()  const { return enModoParry; }
ZonaAtaque Bride::getZonaParry()    const { return zonaParryActual; }
float      Bride::getTiempoParry()  const { return tiempoParryActivo; }

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
    if(estado == Estado::Atacando || estado == Estado::Defendiendo) return;
    estado = Estado::Moviendose;
    dirActual = Direccion::Atras;
    fisicaActual = ModoFisica::dos5Dimensiones;
    acely = 0;
    vely = -FUERZA_ADELANTE_ATRAS;
}

void Bride::moverAdelante()
{
    if(estado == Estado::Atacando || estado == Estado::Defendiendo) return;
    estado = Estado::Moviendose;
    dirActual = Direccion::Adelante;
    fisicaActual = ModoFisica::dos5Dimensiones;
    acely = 0;
    vely = FUERZA_ADELANTE_ATRAS;
}

void Bride::activarParry(ZonaAtaque zona)
{
    if(estado == Estado::Atacando) return;
    enModoParry       = true;
    zonaParryActual   = zona;
    tiempoParryActivo = 0.0f;
    estado            = Estado::Defendiendo;
    zonaActual        = zona;
    actualizarSprite();
}

void Bride::desactivarParry()
{
    enModoParry       = false;
    zonaParryActual   = ZonaAtaque::Inicial;
    tiempoParryActivo = 0.0f;
    if(estado == Estado::Defendiendo)
    {
        estado     = Estado::Quieto;
        zonaActual = ZonaAtaque::Inicial;
        actualizarSprite();
    }
}

void Bride::acelerar()
{
    if(dirActual == Direccion::Derecha)
        velx = std::min(velx + ACELERACION * 0.016f,  VEL_MAX);
    else if(dirActual == Direccion::Izquierda)
        velx = std::max(velx - ACELERACION * 0.016f, -VEL_MAX);
    else if(dirActual == Direccion::Adelante)
        vely = std::max(vely + ACELERACION * 0.016f, -VEL_MAX);
    else if(dirActual == Direccion::Atras)
        vely = std::min(vely - ACELERACION * 0.016f,  VEL_MAX);
}

void Bride::frenar()
{
    if(dirActual == Direccion::Derecha || dirActual == Direccion::Izquierda)
    {
        if(velx > 0) velx = std::max(0.0f, velx - DESACELERACION_TECLA * 0.016f);
        else if(velx < 0)  velx = std::min(0.0f, velx + DESACELERACION_TECLA * 0.016f);
    }
    else if(dirActual == Direccion::Adelante || dirActual == Direccion::Atras)
    {
        if(vely > 0) vely = std::max(0.0f, vely - DESACELERACION_TECLA * 0.016f);
        else if(vely < 0)  vely = std::min(0.0f, vely + DESACELERACION_TECLA * 0.016f);
    }
}

void Bride::moverDerechaXY()
{
    if(estado == Estado::Atacando || estado == Estado::Defendiendo) return;
    estado = Estado::Moviendose;
    dirActual = Direccion::Derecha;
    fisicaActual = ModoFisica::dos5Dimensiones;
    acelx = 0;
    velx = FUERZA_HORIZONTAL;
}

void Bride::moverIzquierdaXY()
{
    if(estado == Estado::Atacando || estado == Estado::Defendiendo) return;
    estado = Estado::Moviendose;
    dirActual = Direccion::Izquierda;
    fisicaActual = ModoFisica::dos5Dimensiones;
    acelx = 0;
    velx = -FUERZA_HORIZONTAL;
}

void Bride::detenerEjeY()
{
    vely = 0;
    acely = 0;
    if(velx == 0 && estado == Estado::Moviendose)
    {
        estado      = Estado::Quieto;
        frameActual = 0;
        tiempoAnimacion = 0;
        actualizarSprite();
    }
}

void Bride::saltarDerecha()
{
    if(!zanshinEspecialActivo || !enSuelo)
        return;

    estado = Estado::Saltando;
    dirActual = Direccion::Derecha;
    fisicaActual = ModoFisica::Parabolica;

    velx = VEL_HORIZONTAL_PARABOLICO;
    vely = -FUERZA_SALTO_PARABOLICO;
    acely = GRAVEDAD_PARABOLICO;
    enSuelo = false;
}

void Bride::saltarIzquierda()
{
    if(!zanshinEspecialActivo || !enSuelo)
        return;

    estado = Estado::Saltando;
    dirActual = Direccion::Izquierda;
    fisicaActual = ModoFisica::Parabolica;

    velx = -VEL_HORIZONTAL_PARABOLICO;
    vely = -FUERZA_SALTO_PARABOLICO;
    acely = GRAVEDAD_PARABOLICO;
    enSuelo = false;
}

void Bride::detenerMovimiento()
{
    estado= Estado::Quieto;
    zonaActual= ZonaAtaque::Inicial;
    velx=0;
    frameActual=0;
    tiempoAnimacion = 0;
    actualizarSprite();
}




