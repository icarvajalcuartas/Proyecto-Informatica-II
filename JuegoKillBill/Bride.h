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
    const float FUERZA_HORIZONTAL = 300.0f;
    void movRectilineo(float difTiempo);
    void movSaltoY(float difTiempo);
    void movParabolico(float difTiempo);
    void movDosDimensiones(float difTiempo);
public:
    Bride(float posx,float posy);
    Bride(const Bride& copiaBride);
    QRectF boundingRect() const override;
    void paint(QPainter* painter,const QStyleOptionGraphicsItem* option,QWidget* widget) override;
    void setModoFisica(ModoFisica modo);
    void setDireccion(Direccion dir);
    void setEnSuelo(bool suelo);
    void realizarParry();
    void saltar();
    void input();
    void actualizar(float difTiempo) override;
    void iniciarAtaque(ZonaAtaque zona)override;
    void esAtaqueValido(ZonaAtaque zona);
    bool getEnSuelo() const;
    unsigned short int getContZanshin() const;
};

#endif // BRIDE_H
