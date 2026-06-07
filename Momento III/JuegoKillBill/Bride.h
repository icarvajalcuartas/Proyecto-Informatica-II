#ifndef BRIDE_H
#define BRIDE_H
#include "Personaje.h"
enum class ModoFisica{Uniforme, Parabolica, dos5Dimensiones, SaltoY};

class Bride:public Personaje{
    Q_OBJECT;
private:
    unsigned short int contZanshin;
    bool zanshinEspecialActivo;
    float tiempoZanshinEspecial;
    ModoFisica fisicaActual;
    bool enSuelo;
    const float DUR_ZANSHINESPECIAL = 10.0f;
    const float FUERZA_SALTO = 500.0f;
    const float FUERZA_HORIZONTAL = 50.0f;
    const float FUERZA_SALTO_PARABOLICO = 600.0f;
    const float VEL_HORIZONTAL_PARABOLICO = 400.0f;
    const float GRAVEDAD_PARABOLICO = 500.0f;
    const float SUELO_Y = 400.0f;
    float tiempoGolpe;
    void movRectilineo(float difTiempo);
    void movSaltoY(float difTiempo);
    void movParabolico(float difTiempo);
    void movDosDimensiones(float difTiempo);
    float tiempoAnimacion;
    const float DURACION_FRAME = 0.1f;
    const float DURACION_GOLPE = 0.4f;
    ZonaAtaque ultimoAtaquevalido;

    const float FUERZA_ADELANTE_ATRAS = 50.0f;
    const float VEL_MAX = 200.0f;
    const float ACELERACION  = 400.0f;
    const float DESACELERACION_TECLA = 200.0f;
    const float FRICCION = 80.0f;
    bool enModoParry;
    float tiempoParryActivo;
    const float DUR_PARRY = 0.6f;
    ZonaAtaque zonaParryActual;
    void actualizarParry(float dt);
public:
    Bride(float posx,float posy);
    QRectF boundingRect() const override;
    void paint(QPainter* painter,const QStyleOptionGraphicsItem* option,QWidget* widget) override;
    void actualizar(float difTiempo) override;
    void actualizarZanshin (float dt);
    void actualizarMovimiento (float dt);
    void actualizarAnimacion (float dt);
    void setModoFisica(ModoFisica modo);
    void setDireccion(Direccion dir);
    void setEnSuelo(bool suelo);
    void recibirGolpe() override;
    void iniciarAtaque(ZonaAtaque zona)override;
    void registrarZanshin(ZonaAtaque zona);
    void falloAtaque();
    bool getEnSuelo() const;
    bool getZanshinEspecialActivo() const;
    unsigned short int getContZanshin() const;
    bool getEnModoParry()  const;
    ZonaAtaque getZonaParry() const;
    float getTiempoParry() const;
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
    void activarParry(ZonaAtaque zona);
    void desactivarParry();
    void acelerar();
    void frenar();
    void moverDerechaXY();
    void moverIzquierdaXY();
    void detenerEjeY();
signals:
    void zanshinEspecialIniciado();
    void zanshinEspecialterminado();
    void zanshinActualizado(unsigned short int contadorZ);
};

#endif // BRIDE_H
