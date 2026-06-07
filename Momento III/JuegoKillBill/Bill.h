#ifndef BILL_H
#define BILL_H
#include "Personaje.h"
#include <QRandomGenerator>

enum class AccionBill { Acercarse, Alejarse, Atacar, Esperar };
enum class Dificultad { Aprendiz, Maestro };
class Bill: public Personaje
{
        Q_OBJECT
private:
    float velocidad;
    float acumTiempo;
    float acumSprite;
    float distanciaAmenaza;
    AccionBill accionActual;
    Dificultad dificultad;
    bool  cooldownAtaque;
    float tiempoCooldown;
    const float DUR_COOLDOWN_APRENDIZ = 2.0f;
    const float DUR_COOLDOWN_MAESTRO  = 1.0f;
    bool  preparandoAtaque;
    float tiempoPreparacion;
    const float DUR_PREP_APRENDIZ = 1.2f;
    const float DUR_PREP_MAESTRO  = 0.5f;
    ZonaAtaque zonaPreparada;
    const float VENTANA_PARRY_APRENDIZ = 0.5f;
    const float VENTANA_PARRY_MAESTRO  = 0.2f;
    unsigned short int contAtaquesBride[3]= {0, 0, 0};
    unsigned short int contParrysBride[3]= {0, 0, 0};
    unsigned short int contGolpesExitosos[3]= {0, 0, 0};
    float probabilidades[3]= {0.33f, 0.33f, 0.34f};
    unsigned short totalInteracciones;
    const unsigned short MIN_INTER_APRENDIZ = 2;
    const unsigned short MIN_INTER_MAESTRO  = 1;

    void decidirAccion(Personaje* jugador);
    void ajustarProbabilidades();
    short int  elegirZonaAtaque();
    short int zonaAIndice(ZonaAtaque zona);
public:
    Bill(float posx, float posy, Dificultad dificultad = Dificultad::Aprendiz);
    QRectF boundingRect() const override;
    void paint(QPainter* painter,const QStyleOptionGraphicsItem* option,QWidget* widget) override;
    void actualizar(float dt) override;
    void iniciarAtaque(ZonaAtaque zona) override;
    void recibirGolpe() override;
    void actualizarMovimiento(float dt);
    void actualizarAnimacion(float dt);
    float getVentanaParry() const;
    void registrarAtaqueBride(ZonaAtaque zona);
    void registrarParryBride(ZonaAtaque zona);
    void registrarGolpeExitoso(ZonaAtaque zona);
public slots:
    void activarVulnerabilidad();
signals:
    void entroEnVulnerabilidad();
};

#endif // BILL_H
