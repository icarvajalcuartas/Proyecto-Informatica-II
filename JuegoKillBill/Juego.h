#ifndef JUEGO_H
#define JUEGO_H
#include <QObject>
#include <QGraphicsScene>
#include <QTimer>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QVector>

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
    QGraphicsScene* escena;
    QTimer * timer;
    QMediaPlayer *reproductor;
    QAudioOutput *salidaAudio;

public:
    Juego();
    void inicialNivel(unsigned short int nivel);
    void cambiarEstado(EstadoJuego nuevoEstado);
    unsigned short int getNivelActual() const;
    QGraphicsScene* getEscena() const;
    void reproducirMusica(const QString& ruta);
private slots:
    void actualizarJuego();
};

#endif // JUEGO_H
