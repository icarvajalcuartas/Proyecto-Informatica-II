#include "Bill.h"
#include <QGraphicsScene>
#include <QPainter>
#include <cmath>

Bill::Bill(float posx, float posy, Dificultad dif)
    : Personaje("Bill", posx, posy, 1),velocidad(100.0f),acumTiempo(0.0f),acumSprite(0.0f),
    distanciaAmenaza(300.0f),accionActual(AccionBill::Esperar),
    dificultad(dif), cooldownAtaque(false),tiempoCooldown(0.0f),preparandoAtaque(false),
    tiempoPreparacion(0.0f),zonaPreparada(ZonaAtaque::Inicial),totalInteracciones(0)
{
    seccionarSpritesheet(":/sprites/bill.png");
    actualizarSprite();
}

QRectF Bill::boundingRect() const
{
    return QRectF(0, 0, 128, 128);
}

void Bill::paint(QPainter* painter,
                 const QStyleOptionGraphicsItem* option,
                 QWidget* widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    painter->drawPixmap(0, 0, spriteActual);
}

void Bill::actualizar(float dt)
{
    acumTiempo += dt;
    float durCooldown = (dificultad == Dificultad::Maestro)? DUR_COOLDOWN_MAESTRO: DUR_COOLDOWN_APRENDIZ;
    if(cooldownAtaque)
    {
        tiempoCooldown += dt;
        if(tiempoCooldown >= durCooldown)
        {
            cooldownAtaque = false;
            tiempoCooldown = 0.0f;
        }
    }
    if(preparandoAtaque)
    {
        tiempoPreparacion += dt;
        float durPrep = (dificultad == Dificultad::Maestro)? DUR_PREP_MAESTRO : DUR_PREP_APRENDIZ;

        if(tiempoPreparacion >= durPrep)
        {
            preparandoAtaque  = false;
            tiempoPreparacion = 0.0f;
            iniciarAtaque(zonaPreparada);
        }
    }
    float intervaloDecision = (dificultad == Dificultad::Maestro) ? 0.2f : 0.4f;
    if(acumTiempo >= intervaloDecision)
    {
        acumTiempo = 0.0f;
        if(scene())
        {
            for(QGraphicsItem* item : scene()->items())
            {
                Personaje* p = dynamic_cast<Personaje*>(item);
                if(p && p != this)
                {
                    decidirAccion(p);
                    break;
                }
            }
        }
    }
    actualizarMovimiento(dt);
    actualizarAnimacion(dt);
    actualizarSprite();
}

void Bill::decidirAccion(Personaje* jugador)
{
    if(!jugador) return;
    float dx = jugador->getPosx() - posx;
    float dy = jugador->getPosy() - posy;
    float distancia = std::sqrt(dx*dx + dy*dy);

    if(std::abs(dx) >= std::abs(dy))
        dirActual = (dx > 0) ? Direccion::Derecha : Direccion::Izquierda;
    else
        dirActual = (dy > 0) ? Direccion::Adelante : Direccion::Atras;


    if(cooldownAtaque || preparandoAtaque)
    {
        velx = 0;
        vely = 0;
        estado = Estado::Quieto;
        zonaActual = ZonaAtaque::Inicial;
        return;
    }

    if(jugador->getEstado() == Estado::Atacando && distancia < 150.0f)
    {
        float norm = std::max(std::abs(dx) + std::abs(dy), 0.001f);
        velx = -(dx / norm) * velocidad;
        vely = -(dy / norm) * velocidad;
        estado = Estado::Moviendose;
        zonaActual = ZonaAtaque::Inicial;
        if(std::abs(velx) >= std::abs(vely))
            dirActual = (velx > 0) ? Direccion::Derecha : Direccion::Izquierda;
        else
            dirActual = (vely > 0) ? Direccion::Adelante : Direccion::Atras;
        return;
    }

    if(distancia < distanciaAmenaza)
    {
        velx  = 0;
        vely  = 0;
        int indice = elegirZonaAtaque();
        switch(indice)
        {
        case 0:
            zonaPreparada = ZonaAtaque::Men;
            zonaActual = ZonaAtaque::Men;
            break;
        case 1:
            zonaPreparada = ZonaAtaque::Do;
            zonaActual = ZonaAtaque::Do;
            break;
        default:
            zonaPreparada = ZonaAtaque::Kote;
            zonaActual= ZonaAtaque::Kote;
            break;
        }
        preparandoAtaque = true;
        tiempoPreparacion = 0.0f;
        cooldownAtaque = true;
        tiempoCooldown = 0.0f;
        estado = Estado::Defendiendo;
        return;
    }
    float norm = std::max(std::abs(dx) + std::abs(dy), 0.001f);
    velx  = (dx / norm) * velocidad;
    vely  = (dy / norm) * velocidad;
    estado    = Estado::Moviendose;
    zonaActual = ZonaAtaque::Inicial;

}

void Bill::registrarAtaqueBride(ZonaAtaque zona)
{
    int i = zonaAIndice(zona);
    if(i < 0) return;
    contAtaquesBride[i]++;
    totalInteracciones++;
    unsigned short minInter = (dificultad == Dificultad::Maestro)? MIN_INTER_MAESTRO: MIN_INTER_APRENDIZ;

    if(totalInteracciones >= minInter)
        ajustarProbabilidades();
}

void Bill::registrarParryBride(ZonaAtaque zona)
{
    int i = zonaAIndice(zona);
    if(i < 0) return;
    contParrysBride[i]++;
    totalInteracciones++;
    unsigned short minInter = (dificultad == Dificultad::Maestro)? MIN_INTER_MAESTRO: MIN_INTER_APRENDIZ;
    if(totalInteracciones >= minInter)
        ajustarProbabilidades();
}

void Bill::registrarGolpeExitoso(ZonaAtaque zona)
{
    int i = zonaAIndice(zona);
    if(i < 0) return;
    contGolpesExitosos[i]++;
}

void Bill::ajustarProbabilidades()
{
    float pesos[3];
    float total = 0.0f;

    for(int i = 0; i < 3; i++)
    {
        float ataques = contAtaquesBride[i] + 1.0f;
        float parrys  = contParrysBride[i] + 1.0f;
        float exitosos = contGolpesExitosos[i] + 1.0f;

        if(dificultad == Dificultad::Aprendiz)
        {
            pesos[i] = 1.0f + (ataques * 0.2f);
        }
        else
        {
            float bloqueados = contParrysBride[i] + 1.0f;
            pesos[i] = (ataques * exitosos) / (parrys * bloqueados);
        }
        total += pesos[i];
    }

    for(int i = 0; i < 3; i++)
        probabilidades[i] = pesos[i] / total;
}

short int Bill::elegirZonaAtaque()
{
    float r = static_cast<float>(QRandomGenerator::global()->generateDouble());
    float acum = 0.0f;

    for(int i = 0; i < 3; i++)
    {
        acum += probabilidades[i];
        if(r <= acum)
            return i;
    }
    return 2;
}

short int Bill::zonaAIndice(ZonaAtaque zona)
{
    switch(zona)
    {
    case ZonaAtaque::Men:  return 0;
    case ZonaAtaque::Do:   return 1;
    case ZonaAtaque::Kote: return 2;
    default:               return -1;
    }
}

void Bill::iniciarAtaque(ZonaAtaque zona)
{
    if(estado == Estado::Atacando) return;
    zonaActual  = zona;
    estado = Estado::Atacando;
    frameActual = 0;
    actualizarSprite();
}

void Bill::recibirGolpe()
{
    vida--;
    estado = Estado::Derrotado;
    frameActual = 0;
    actualizarSprite();
}


void Bill::activarVulnerabilidad()
{
    estado = Estado::Vulnerable;
    frameActual = 0;
    actualizarSprite();
    emit entroEnVulnerabilidad();
}

float Bill::getVentanaParry() const
{
    return (dificultad == Dificultad::Maestro)? VENTANA_PARRY_MAESTRO : VENTANA_PARRY_APRENDIZ;
}

void Bill::actualizarMovimiento(float dt)
{
    posx += velx * dt;
    posy += vely * dt;
    setPos(posx, posy);
}


void Bill::actualizarAnimacion(float dt)
{
    acumSprite += dt;
    if(acumSprite < 0.15f) return;
    acumSprite = 0.0f;

    QVector<QPixmap>* frames = obtenerSpriteActual();
    if(!frames || frames->isEmpty()) return;

    switch(estado)
    {
    case Estado::Atacando:
        frameActual++;
        if(frameActual >= frames->size())
        {
            frameActual = 0;
            zonaActual = ZonaAtaque::Inicial;
            estado = Estado::Quieto;
        }
        break;

    case Estado::Golpeado:
        frameActual++;
        if(frameActual >= frames->size())
        {
            frameActual = 0;
            estado = Estado::Quieto;
        }
        break;

    case Estado::Derrotado:
        if(frameActual < frames->size() - 1)
            frameActual++;
        break;

    case Estado::Vulnerable:
        frameActual = (frameActual + 1) % frames->size();
        break;

    default:
        frameActual = (frameActual + 1) % frames->size();
        break;
    }
}
