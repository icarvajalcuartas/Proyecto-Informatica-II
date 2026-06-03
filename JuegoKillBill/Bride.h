#ifndef BRIDE_H
#define BRIDE_H
#include "Personaje.h"
enum class ModoFisica{Uniforme, Parabolica, dos5Dimensiones, SaltoY};

class Bride:public Personaje{
private:
    unsigned short int contZanshin;
    unsigned short int puntaje;
    bool ataqueValido;
    ModoFisica fisicaActual;
    bool enSuelo;
    const float FUERZA_SALTO = 500.0f;
    const float FUERZA_HORIZONTAL = 100.0f;
    void movRectilineo();
    void movSaltoY(float difTiempo);
    void movParabolico(float difTiempo);
    void movDosDimensiones(float difTiempo);
    float tiempoAnimacion =0;
    const float DURACION_FRAME = 0.1f;
public:
    Bride(float posx,float posy);
    Bride(const Bride& copiaBride);
    QRectF boundingRect() const override;
    void paint(QPainter* painter,const QStyleOptionGraphicsItem* option,QWidget* widget) override;
    virtual QRectF getHitboxAtaque() const override;
    void setModoFisica(ModoFisica modo);
    void setDireccion(Direccion dir);
    void setEnSuelo(bool suelo);
    void realizarParry();
    void input();
    void actualizar(float difTiempo) override;
    void iniciarAtaque(ZonaAtaque zona)override;
    void esAtaqueValido(ZonaAtaque zona);
    bool getEnSuelo() const;
    unsigned short int getContZanshin() const;
public slots:
    void moverDerecha();
    void moverIzquierda();
    void moverAtras();
    void moverAdelante();
    void detenerMovimiento();
    void saltar();
    void ataqueMen();
    void ataqueDo();
    void ataqueKote();
};

#endif // BRIDE_H
