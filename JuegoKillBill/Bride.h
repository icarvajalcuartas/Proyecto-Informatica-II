#ifndef BRIDE_H
#define BRIDE_H
#include "Personaje.h"
enum class ModoFisica{Uniforme, Parabolica, dos5Dimensiones, SaltoY};

class Bride:public Personaje{
    Q_OBJECT;
private:
    unsigned short int contZanshin;
    bool zanshinEspecialActivo = false;
    float tiempoZanshinEspecial = 0.0f;
    const float DUR_ZANSHINESPECIAL = 10.0f;
    unsigned short int puntaje;
    bool ataqueValido;
    ModoFisica fisicaActual;
    bool enSuelo;
    const float FUERZA_SALTO = 500.0f;
    const float FUERZA_HORIZONTAL = 50.0f;
    void movRectilineo(float difTiempo);
    void movSaltoY(float difTiempo);
    void movParabolico(float difTiempo);
    void movDosDimensiones(float difTiempo);
    float tiempoAnimacion =0;
    const float DURACION_FRAME = 0.1f;
    ZonaAtaque ultimoAtaquevalido;
public:
    Bride(float posx,float posy);
    //Bride(const Bride& copiaBride);
    QRectF boundingRect() const override;
    void paint(QPainter* painter,const QStyleOptionGraphicsItem* option,QWidget* widget) override;
    virtual QRectF getHitboxAtaque() const override;
    void actualizar(float difTiempo) override;
    void actualizarZanshin (float dt);
    // void actualizarEstadosAtaque(float dt);
    void actualizarMovimiento (float dt);
    void actualizarAnimacion (float dt);
    void sumarPunto();
    void setModoFisica(ModoFisica modo);
    void setDireccion(Direccion dir);
    void setEnSuelo(bool suelo);
    void realizarParry();
    void recibirGolpe() override;
    void input();
    void iniciarAtaque(ZonaAtaque zona)override;
    void registrarZanshin(ZonaAtaque zona);
    void falloAtaque();

    bool getEnSuelo() const;
    bool getZanshinEspecialActivo() const;
    unsigned short int getContZanshin() const;
public slots:
    void moverDerecha();
    void moverIzquierda();
    void moverAtras();
    void moverAdelante();
    void saltarDerecha();
    void saltarIzquierda();
    void detenerMovimiento();
    void saltar();
    void ataqueMen();
    void ataqueDo();
    void ataqueKote();
signals:
    void zanshinEspecialIniciado();
    void zanshinEspecialterminado();
    void zanshinActualizado(unsigned short int contadorZ);
};

#endif // BRIDE_H
