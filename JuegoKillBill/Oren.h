#ifndef OREN_H
#define OREN_H
#include "Personaje.h"
#include <QTimer>

enum class Accion{
    Acercarse,Retroceder,Atacar,Esperar
};

class Oren: public Personaje
{
    Q_OBJECT
private:
    float distanciaAmenaza;
    float distanciaRetroceder;
    float velocidad;
    float acumTiempo;
    float acumSprite;
    Accion accionActual;
    bool modoDebilitado = false;


    bool cooldownAtaque;
    float tiempoCooldown;
    void decidirAccion(Personaje* jugador);
public:
    Oren(float posx, float posy);
    QRectF boundingRect() const override;
    void paint(QPainter* painter,const QStyleOptionGraphicsItem* option,QWidget* widget) override;
    void actualizar(float difTiempo) override;
    void iniciarAtaque(ZonaAtaque zona) override;
    virtual void recibirGolpe() override;
    void actualizarMovimiento(float dt);
    void actualizarAnimacion(float dt);
    void activarModoDebilitado();
    void desactivarModoDebilitado();


};

#endif // OREN_H
