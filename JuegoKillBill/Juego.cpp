#include "Juego.h"

bool Juego::eventFilter(QObject *obj, QEvent *event)
{
    if(event->type() == QEvent::KeyPress)
    {
        QKeyEvent* tecla = static_cast<QKeyEvent*>(event);
        if(tecla->isAutoRepeat())
            return true;

        if(nivelActual < niveles.size())
        {
            niveles[nivelActual]->inputJugador(tecla);
        }
    }
    else if(event->type() == QEvent::KeyRelease)
    {
        QKeyEvent* tecla = static_cast<QKeyEvent*>(event);
        if(tecla->isAutoRepeat())
            return true;

        niveles[nivelActual]->inputJugadorLiberada(tecla);
    }

    return QObject::eventFilter(obj,event);
}
void Juego::actualizarJuego()
{
    if(nivelActual < niveles.size())
    {
        niveles[nivelActual]->actualizar(0.016f);
    }
}
Juego::Juego() :nivelActual(0),estadoJuegoActual(EstadoJuego::Menu){

    escena = new QGraphicsScene(this);
    timer = new QTimer(this);
    reproductor = new QMediaPlayer(this);
    salidaAudio = new QAudioOutput(this);
    reproductor->setAudioOutput(salidaAudio);
    niveles.append(new Nivel(1, escena));
    connect(timer,&QTimer::timeout,this,&Juego::actualizarJuego);
    timer->start(16);
    inicialNivel(0);
}

void Juego::inicialNivel(unsigned short nivel)
{
    if(nivel >= niveles.size())
        return;
    escena->clear();
    nivelActual = nivel;
    niveles[nivelActual]->cargar();
}

unsigned short int Juego::getNivelActual() const
{
    return nivelActual;
}
void Juego::reproducirMusica(const QString& ruta)
{
    reproductor->setSource(QUrl(ruta));
    reproductor->play();
}
QGraphicsScene* Juego::getEscena() const
{
    return escena;
}
Juego::~Juego()
{
    qDeleteAll(niveles);
    niveles.clear();
}


