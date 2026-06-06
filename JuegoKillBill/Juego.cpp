#include "Juego.h"
#include <QPushButton>
#include <QGraphicsProxyWidget>
#include <QPixmap>
#include <QKeyEvent>
#include <QGraphicsView>
Juego::Juego() :nivelActual(0),estadoJuegoActual(EstadoJuego::Menu),vista(nullptr){

    escenaMenu = new QGraphicsScene(this);
    escenaJuego = new QGraphicsScene(this);
    timer = new QTimer(this);
    //niveles.append(new Nivel(1, escenaJuego));
    // niveles.append(new Nivel(2, escenaJuego));
    connect(timer,&QTimer::timeout,this,&Juego::actualizarJuego);
    timer->start(16);
    musicaMenu  = new QMediaPlayer(this);
    salidaMenu  = new QAudioOutput(this);
    musicaMenu->setAudioOutput(salidaMenu);
    salidaMenu->setVolume(0.7f);
    musicaMenu->setSource(QUrl("qrc:/audio/Bangbang.mp3"));
    musicaMenu->setLoops(QMediaPlayer::Infinite);

    musicaNivel = new QMediaPlayer(this);
    salidaNivel = new QAudioOutput(this);
    musicaNivel->setAudioOutput(salidaNivel);
    salidaNivel->setVolume(0.7f);
    musicaNivel->setLoops(QMediaPlayer::Infinite);
    qDebug() << "Niveles creados:" << niveles.size();
}

void Juego::iniciarMenu(QGraphicsView *ventana)
{
    vista = ventana;
    QPixmap fondo(":/fondo/fondomenu.png");
    qDebug() << "Fondo menu cargado:" << !fondo.isNull();
    escenaMenu->addPixmap(fondo);
    escenaMenu->setSceneRect(0, 0, 1200, 630);
    qDebug() << "Paso 1: fondo OK";

    QPushButton* botonPlay = new QPushButton();
    botonPlay->setFixedSize(150, 60);
    botonPlay->setCursor(Qt::PointingHandCursor);
    botonPlay->setStyleSheet(
        "QPushButton {"
        "  background-color: transparent;"
        "  border: none;"
        "}"
        "QPushButton:hover {"
        "  background-color: rgba(255,255,255,40);"
        "  border-radius: 6px;"
        "}"
        );
    qDebug() << "Paso 2: botonPlay creado";
    QGraphicsProxyWidget* proxyPlay = escenaMenu->addWidget(botonPlay);
    qDebug() << "Paso 3: proxyPlay agregado";
    proxyPlay->setPos(700, 320);
    qDebug() << "Paso 4: proxyPlay posicionado";



    QPushButton* botonLevel1 = new QPushButton();
    botonLevel1->setFixedSize(150, 55);
    botonLevel1->setCursor(Qt::PointingHandCursor);
    botonLevel1->setStyleSheet(botonPlay->styleSheet());
    QGraphicsProxyWidget* proxyLevel1 = escenaMenu->addWidget(botonLevel1);
    proxyLevel1->setPos(500, 450);
    qDebug() << "Paso 5: proxyLevel1 posicionado";


    QPushButton* botonLevel2 = new QPushButton();
    botonLevel2->setFixedSize(150, 55);
    botonLevel2->setCursor(Qt::PointingHandCursor);
    botonLevel2->setStyleSheet(botonPlay->styleSheet());
    QGraphicsProxyWidget* proxyLevel2 = escenaMenu->addWidget(botonLevel2);
    proxyLevel2->setPos(700, 450);
    qDebug() << "Paso 6: proxyLevel2 posicionado";

    connect(botonPlay, &QPushButton::clicked, this, [this](){
        inicialNivel(0);
    });

    connect(botonLevel1, &QPushButton::clicked, this, [this](){
        inicialNivel(0);
    });

    connect(botonLevel2, &QPushButton::clicked, this, [this](){
        inicialNivel(1);
    });
    qDebug() << "Paso 7: conexiones OK";
    vista->setScene(escenaMenu);
    qDebug() << "SceneRect:" << escenaMenu->sceneRect();
    qDebug() << "Items en escena:" << escenaMenu->items().size();
    qDebug() << "Paso 8: escena asignada";
    qDebug() << "Ruta musica menu:" << musicaMenu->source();
    musicaMenu->play();
    qDebug() << "Estado reproductor:" << musicaMenu->playbackState();
    qDebug() << "Paso 9: musica iniciada";
    estadoJuegoActual = EstadoJuego::Menu;
    qDebug() << "Paso 10: estado Menu OK";


}
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
        if(nivelActual < (unsigned short)niveles.size()) {
        niveles[nivelActual]->inputJugadorLiberada(tecla);
        }
    }

    return QObject::eventFilter(obj,event);
}
void Juego::actualizarJuego()
{
    if(estadoJuegoActual == EstadoJuego::Victoria ||
        estadoJuegoActual == EstadoJuego::Derrota)
        return;
    if(nivelActual < niveles.size())
    {
        niveles[nivelActual]->actualizar(0.016f);
    }
}


void Juego::inicialNivel(unsigned short nivel)
{
    musicaMenu->stop();

    escenaJuego->clear();

    Nivel* nuevoNivel = new Nivel(nivel + 1, escenaJuego);

    if(nivelActual < (unsigned short)niveles.size())
    {
        delete niveles[nivelActual];
        niveles[nivelActual] = nuevoNivel;
    }
    else
    {
        niveles.append(nuevoNivel);
    }
    nivelActual = nivel;
    niveles[nivelActual]->cargar();

    if(nivel == 0)
        musicaNivel->setSource(QUrl("qrc:/audio/Lonely.mp3"));
    // else
    //     musicaNivel->setSource(QUrl("qrc:/audio/nivel2.mp3"));
    musicaNivel->play();

    connect(niveles[nivelActual], &Nivel::victoria,
            this, [this](){ cambiarEstado(EstadoJuego::Victoria); });
    connect(niveles[nivelActual], &Nivel::derrota,
            this, [this](){ cambiarEstado(EstadoJuego::Derrota); });

    vista->setScene(escenaJuego);
    vista->setSceneRect(0, 0, 1200, 630);
    ajustarVista();
    estadoJuegoActual = (nivel == 0) ? EstadoJuego::Nivel1 : EstadoJuego::Nivel2;

    timer->start(16);

}

void Juego::ajustarVista()
{
    if(vista)
        vista->fitInView(0, 0, 1200, 630, Qt::IgnoreAspectRatio);
}

void Juego::cambiarEstado(EstadoJuego nuevoEstado)
{
    estadoJuegoActual = nuevoEstado;

    switch(nuevoEstado)
    {
    case EstadoJuego::Victoria:
    {
        timer->stop();
        musicaNivel->stop();
        escenaJuego->clear();

        QGraphicsScene* escenaFinal = new QGraphicsScene(this);
        QPixmap img(":/fondo/fondoVictoria.png");
        escenaFinal->addPixmap(img);
        escenaFinal->setSceneRect(0, 0, 1200, 630);
        vista->setScene(escenaFinal);
        vista->setSceneRect(0, 0, 1200, 630);
        ajustarVista();
        break;
    }
    case EstadoJuego::Derrota:
    {
        timer->stop();
        musicaNivel->stop();
        escenaJuego->clear();

        QGraphicsScene* escenaFinal = new QGraphicsScene(this);
        QPixmap img(":/fondo/fondoDerrota.png");
        escenaFinal->addPixmap(img);
        escenaFinal->setSceneRect(0, 0, 1200, 630);
        vista->setScene(escenaFinal);
        ajustarVista();
        break;
    }
    default:
        break;
    }

}

unsigned short int Juego::getNivelActual() const
{
    return nivelActual;
}
Juego::~Juego()
{
    qDeleteAll(niveles);
    niveles.clear();
}


