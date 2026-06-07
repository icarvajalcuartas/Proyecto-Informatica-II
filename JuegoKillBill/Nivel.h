#ifndef NIVEL_H
#define NIVEL_H
#include "Bride.h"
#include "Oren.h"
#include "Bill.h"
#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QEvent>
#include <QKeyEvent>
#include <QSet>
#include <QGraphicsRectItem>
#include <QSoundEffect>


class Nivel:public QObject
{
    Q_OBJECT
private:
    unsigned short int numeroNivel;
    QGraphicsScene* escena;
    Bride* bride;
    Oren* oren;
    Bill*  bill;
    unsigned short puntosBride;
    unsigned short puntosOren;
    bool terminado;
    bool colisionAtaqueBride;
    bool colisionAtaqueOren;
    const unsigned short MAX_PUNTOS = 5;
    bool colisionAtaqueBill;
    bool parryExitosoActivo = false;
    Dificultad dificultadBill = Dificultad::Aprendiz;
    const float BORDE_IZQUIERDO = 0.0f;
    const float BORDE_DERECHO = 1020.0f;
    const float BORDE_ARRIBA = 300.0f;
    const float BORDE_ABAJO = 400.0f;
    const float DISTANCIA_MINIMA = 70.0f;
    const float POS_Y_PERSONAJES = 400.0f;
    const float POS_X_BRIDE = 600.0f;
    const float POS_X_OREN = 300.0f;
    const float POS_X_BILL = 200.0f;
    const float Y_MIN_ESCALA = 200.0f;
    const float Y_MAX_ESCALA = 550.0f;
    const float ESCALA_MIN = 1.2f;
    const float ESCALA_MAX = 2.0f;
    bool acelerandoActivo;
    bool frenandoActivo;

    QGraphicsTextItem* textoPuntosBride;
    QGraphicsTextItem* textoPuntosOren;
    QGraphicsTextItem* textoZanshin;

    QSet<int> teclasActivas;
    QSoundEffect* sonidoGolpe;
    QSoundEffect* sonidoGritoBride;
    QGraphicsRectItem* bannerZanshin;
    QGraphicsTextItem* textoBanner;
    void cargarNivel1();
    void cargarNivel2();
    void verificarBordes();
    void verificarColisiones();
    void verificarColisionesNivel2();
    void verificarSolapamiento();
    void actualizarMarcador();
    void actualizarEscala();

public:

    Nivel(unsigned short int numero, QGraphicsScene* escena);
    void cargar();
    void inputJugador(QKeyEvent* evento);
    void inputJugadorLiberada(QKeyEvent *evento);
    void actualizar(float dt);
    bool getTerminado() const;
    unsigned short int getNumeroNivel() const;
    Bride* getBride() const;
    void setDificultad(Dificultad dif);
signals:
    void derrota();
    void victoria();
};

#endif // NIVEL_H
