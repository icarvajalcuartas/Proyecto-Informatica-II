#ifndef JUEGO_H
#define JUEGO_H
#include <QObject>
#include <QGraphicsScene>
#include <QTimer>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QVector>
#include "Nivel.h"

enum class EstadoJuego
{
    Menu,
    Cargando,
    Nivel1,
    Nivel2,
    Victoria,
    Derrota
};
class Juego: public QObject
{
    Q_OBJECT
private:
    unsigned short int nivelActual;
    EstadoJuego estadoJuegoActual;
    QGraphicsView* vista;
    QGraphicsScene* escenaMenu;
    QGraphicsScene* escenaJuego;
    QGraphicsScene* escenaCarga;
    QGraphicsScene* escenaDerrota;
    QTimer * timer;
    QMediaPlayer* musicaMenu;
    QAudioOutput* salidaMenu;
    QMediaPlayer* musicaNivel;
    QAudioOutput* salidaNivel;
    QVector<Nivel*> niveles;
protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
public:
    Juego();
    void iniciarMenu(QGraphicsView* ventana);
    void inicialNivel(unsigned short int nivel);
    void ajustarVista();
    unsigned short int getNivelActual() const;
    ~Juego();
private slots:
    void actualizarJuego();

public slots:
    void cambiarEstado(EstadoJuego nuevoEstado);
signals:
    void juegoTerminadoVictoria();
    void juegoTerminadoDerrota();
    void cambiarEscena(QGraphicsScene* escena);
};

#endif // JUEGO_H
