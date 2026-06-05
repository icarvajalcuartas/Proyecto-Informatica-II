#ifndef OREN_H
#define OREN_H
#include "Personaje.h"
#include <QTimer>

class Oren: public Personaje
{
    Q_OBJECT
private:
    float distanciaAmenaza;
    float distanciaRetroceder;
    float velocidad;
    QTimer timerOren;
    float acumTiempo;
    void decidirAccion(Personaje* jugador);
public:
    Oren(float posx, float posy);
    QRectF boundingRect() const override;
    void paint(QPainter* painter,const QStyleOptionGraphicsItem* option,QWidget* widget) override;
    void actualizar(float difTiempo) override;
    void iniciarAtaque(ZonaAtaque zona) override;
    virtual void recibirGolpe() override;


};

#endif // OREN_H
