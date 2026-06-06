#include "Juego.h"
#include <QPushButton>
#include <QGraphicsProxyWidget>
#include <QPixmap>
#include <QKeyEvent>
#include <QGraphicsView>
Juego::Juego() :nivelActual(0),estadoJuegoActual(EstadoJuego::Menu),vista(nullptr){

    escenaMenu = new QGraphicsScene(this);
    escenaJuego = new QGraphicsScene(this);
    escenaCarga  = new QGraphicsScene(this);
    escenaDerrota = new QGraphicsScene(this);

    timer = new QTimer(this);
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
}

void Juego::iniciarMenu(QGraphicsView *ventana)
{
    vista = ventana;
    QPixmap fondo(":/fondo/fondomenu.png");
    escenaMenu->addPixmap(fondo);
    escenaMenu->setSceneRect(0, 0, 1200, 630);

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
    QGraphicsProxyWidget* proxyPlay = escenaMenu->addWidget(botonPlay);
    proxyPlay->setPos(670, 320);



    QPushButton* botonLevel1 = new QPushButton();
    botonLevel1->setFixedSize(150, 55);
    botonLevel1->setCursor(Qt::PointingHandCursor);
    botonLevel1->setStyleSheet(botonPlay->styleSheet());
    QGraphicsProxyWidget* proxyLevel1 = escenaMenu->addWidget(botonLevel1);
    proxyLevel1->setPos(500, 470);


    QPushButton* botonLevel2 = new QPushButton();
    botonLevel2->setFixedSize(150, 55);
    botonLevel2->setCursor(Qt::PointingHandCursor);
    botonLevel2->setStyleSheet(botonPlay->styleSheet());
    QGraphicsProxyWidget* proxyLevel2 = escenaMenu->addWidget(botonLevel2);
    proxyLevel2->setPos(850, 460);


    connect(botonPlay, &QPushButton::clicked, this, [this](){
        inicialNivel(0);
    });

    connect(botonLevel1, &QPushButton::clicked, this, [this](){
        inicialNivel(0);
    });

    connect(botonLevel2, &QPushButton::clicked, this, [this](){
        inicialNivel(1);
    });

    vista->setScene(escenaMenu);
    musicaMenu->play();
    estadoJuegoActual = EstadoJuego::Menu;


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
    escenaCarga->clear();

    QPixmap imgCarga;
    if(nivel == 0)
        imgCarga = QPixmap(":/fondo/cargaNivel1.png");
    else
        imgCarga = QPixmap(":/fondo/cargaNivel2.png");

    qDebug() << "Carga nivel" << nivel << "cargada:" << !imgCarga.isNull();
    escenaCarga->addPixmap(imgCarga);
    escenaCarga->setSceneRect(0, 0, 1200, 630);
    vista->setScene(escenaCarga);
    ajustarVista();
    QTimer::singleShot(2000, this, [this, nivel](){
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

        connect(niveles[nivelActual], &Nivel::victoria,
                this, [this](){ cambiarEstado(EstadoJuego::Victoria); });
        connect(niveles[nivelActual], &Nivel::derrota,
                this, [this](){ cambiarEstado(EstadoJuego::Derrota); });

        vista->setScene(escenaJuego);
        vista->setSceneRect(0, 0, 1200, 630);
        ajustarVista();
        estadoJuegoActual = (nivel == 0) ? EstadoJuego::Nivel1 : EstadoJuego::Nivel2;
        timer->start(16);

        disconnect(musicaNivel, nullptr, nullptr, nullptr);
        musicaNivel->setLoops(QMediaPlayer::Infinite);

        if(nivel == 0)
            musicaNivel->setSource(QUrl("qrc:/audio/Lonely.wav"));

        QTimer::singleShot(200, this, [this](){ musicaNivel->play(); });
    });
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

        escenaDerrota->clear();
        QPixmap img(":/fondo/fondoDerrota.png");
        escenaDerrota->addPixmap(img);
        escenaDerrota->setSceneRect(0, 0, 1200, 630);

        QPushButton* btnTryAgain = new QPushButton();
        btnTryAgain->setFixedSize(230, 80);
        btnTryAgain->setCursor(Qt::PointingHandCursor);
        btnTryAgain->setStyleSheet(
            "QPushButton { background-color: transparent; border: none; }"
            "QPushButton:hover { background-color: rgba(255,255,255,40); border-radius:6px; }"
            );
        QGraphicsProxyWidget* proxyTry = escenaDerrota->addWidget(btnTryAgain);
        proxyTry->setPos(380, 450);


        QPushButton* btnMenu = new QPushButton();
        btnMenu->setFixedSize(210, 80);
        btnMenu->setCursor(Qt::PointingHandCursor);
        btnMenu->setStyleSheet(btnTryAgain->styleSheet());
        QGraphicsProxyWidget* proxyMenu = escenaDerrota->addWidget(btnMenu);
        proxyMenu->setPos(650, 450);


        connect(btnTryAgain, &QPushButton::clicked, this, [this](){
            estadoJuegoActual = EstadoJuego::Menu;
            inicialNivel(nivelActual);
        });

        connect(btnMenu, &QPushButton::clicked, this, [this](){
            musicaNivel->stop();
            estadoJuegoActual = EstadoJuego::Menu;
            vista->setScene(escenaMenu);
            ajustarVista();
            musicaMenu->play();
        });

        vista->setScene(escenaDerrota);
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


