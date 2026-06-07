#include "Juego.h"
#include <QPushButton>
#include <QGraphicsProxyWidget>
#include <QPixmap>
#include <QKeyEvent>
#include <QGraphicsView>
#include <QApplication>
Juego::Juego() :nivelActual(0),estadoJuegoActual(EstadoJuego::Menu),vista(nullptr),
    dificultadNivel2(Dificultad::Aprendiz),soloPintar(false)
{

    escenaMenu = new QGraphicsScene(this);
    escenaJuego = new QGraphicsScene(this);
    escenaCarga = new QGraphicsScene(this);
    escenaDerrota = new QGraphicsScene(this);
    escenaDificultad = new QGraphicsScene(this);

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
    escenaMenu->clear();
    QPixmap fondo(":/fondo/fondomenu2.png");
    escenaMenu->addPixmap(fondo);
    escenaMenu->setSceneRect(0, 0, 1200, 630);
    QString estilo =
        "QPushButton { background-color: transparent; border: none; }"
        "QPushButton:hover { background-color: rgba(255,255,255,40); border-radius:6px; }";

    QPushButton* botonPlay = new QPushButton();
    botonPlay->setFixedSize(150, 60);
    botonPlay->setCursor(Qt::PointingHandCursor);
    botonPlay->setStyleSheet(estilo);
    escenaMenu->addWidget(botonPlay)->setPos(675, 250);

    QPushButton* botonLevel1 = new QPushButton();
    botonLevel1->setFixedSize(150, 65);
    botonLevel1->setCursor(Qt::PointingHandCursor);
    botonLevel1->setStyleSheet(estilo);
    escenaMenu->addWidget(botonLevel1)->setPos(480, 330);


    QPushButton* botonLevel2 = new QPushButton();
    botonLevel2->setFixedSize(150, 65);
    botonLevel2->setCursor(Qt::PointingHandCursor);
    botonLevel2->setStyleSheet(estilo);
    escenaMenu->addWidget(botonLevel2)->setPos(880, 330);

    QPushButton* botonFacil = new QPushButton();
    botonFacil->setFixedSize(150, 60);
    botonFacil->setCursor(Qt::PointingHandCursor);
    botonFacil->setStyleSheet(estilo);
    escenaMenu->addWidget(botonFacil)->setPos(770, 450);

    QPushButton* botonHardcore = new QPushButton();
    botonHardcore->setFixedSize(150, 60);
    botonHardcore->setCursor(Qt::PointingHandCursor);
    botonHardcore->setStyleSheet(estilo);
    escenaMenu->addWidget(botonHardcore)->setPos(960, 450);

    QPushButton* botonExit = new QPushButton();
    botonExit->setFixedSize(140, 65);
    botonExit->setCursor(Qt::PointingHandCursor);
    botonExit->setStyleSheet(estilo);
    escenaMenu->addWidget(botonExit)->setPos(360, 530);

    connect(botonPlay, &QPushButton::clicked, this, [this](){
        inicialNivel(0);
    });

    connect(botonLevel1, &QPushButton::clicked, this, [this](){
        inicialNivel(0);
    });

    connect(botonLevel2, &QPushButton::clicked, this, [this](){
        inicialNivel(1);
    });
    connect(botonFacil, &QPushButton::clicked, this, [this](){
        dificultadNivel2 = Dificultad::Aprendiz;
        inicialNivel(1);
    });
    connect(botonHardcore, &QPushButton::clicked, this, [this](){
        dificultadNivel2 = Dificultad::Maestro;
        inicialNivel(1);
    });
    connect(botonExit, &QPushButton::clicked, this, [](){
        QApplication::quit();
    });

    vista->setScene(escenaMenu);
    musicaMenu->play();
    estadoJuegoActual = EstadoJuego::Menu;


}
void Juego::mostrarSeleccionDificultad()
{
    escenaDificultad->clear();

    QPixmap fondo(":/fondo/eleccionDificultad.png");
    escenaDificultad->addPixmap(fondo);
    escenaDificultad->setSceneRect(0, 0, 1200, 630);

    QString estilo =
        "QPushButton { background-color: transparent; border: none; }"
        "QPushButton:hover { background-color: rgba(255,255,255,40); border-radius:6px; }";


    QPushButton* btnAprendiz = new QPushButton();
    btnAprendiz->setFixedSize(190, 60);
    btnAprendiz->setCursor(Qt::PointingHandCursor);
    btnAprendiz->setStyleSheet(estilo);
    escenaDificultad->addWidget(btnAprendiz)->setPos(380, 320);

    QPushButton* btnMaestro = new QPushButton();
    btnMaestro->setFixedSize(180, 60);
    btnMaestro->setCursor(Qt::PointingHandCursor);
    btnMaestro->setStyleSheet(estilo);
    escenaDificultad->addWidget(btnMaestro)->setPos(610, 320);

    QPushButton* btnVolver = new QPushButton();
    btnVolver->setFixedSize(170, 90);
    btnVolver->setCursor(Qt::PointingHandCursor);
    btnVolver->setStyleSheet(estilo);
    escenaDificultad->addWidget(btnVolver)->setPos(500, 430);

    connect(btnAprendiz, &QPushButton::clicked, this, [this](){
        dificultadNivel2 = Dificultad::Aprendiz;
        inicialNivel(1);
    });
    connect(btnMaestro, &QPushButton::clicked, this, [this](){
        dificultadNivel2 = Dificultad::Maestro;
        inicialNivel(1);
    });
    connect(btnVolver, &QPushButton::clicked, this, [this](){
        vista->setScene(escenaMenu);
        ajustarVista();
    });

    vista->setScene(escenaDificultad);
    ajustarVista();
}

bool Juego::eventFilter(QObject *obj, QEvent *event)
{
    if(estadoJuegoActual != EstadoJuego::Nivel1 &&
        estadoJuegoActual != EstadoJuego::Nivel2)
        return QObject::eventFilter(obj, event);

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
    if(soloPintar)
    {
        if(nivelActual < (unsigned short)niveles.size())
        {
            if(niveles[nivelActual]->getBride())
                niveles[nivelActual]->getBride()->update();
        }
        return;
    }
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
    timer->stop();
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

        if(nivel < (unsigned short)niveles.size())
        {
            delete niveles[nivel];
            niveles[nivel] = nuevoNivel;
        }
        else
        {
            while((unsigned short)niveles.size() < nivel)
                niveles.append(nullptr);
            niveles.append(nuevoNivel);
        }
        nivelActual = nivel;
        if(nivel == 1)
            niveles[nivelActual]->setDificultad(dificultadNivel2);
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
        else
            musicaNivel->setSource(QUrl("qrc:/audio/nivel2.wav"));
        qDebug() << "Fuente nivel" << nivel << ":" << musicaNivel->source();
        qDebug() << "Estado media:" << musicaNivel->mediaStatus();
        connect(musicaNivel, &QMediaPlayer::mediaStatusChanged,
                this, [this](QMediaPlayer::MediaStatus status){
                    if(status == QMediaPlayer::EndOfMedia)
                        musicaNivel->play();
                });

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

        soloPintar = true;
        musicaNivel->stop();
        QTimer::singleShot(2500, this, [this](){
            soloPintar = false;
            timer->stop();
            escenaJuego->clear();

            QGraphicsScene* escenaFinal = new QGraphicsScene(this);
            QPixmap img;
            if(nivelActual == 0)
                img = QPixmap(":/fondo/fondovictoriaNivel1.png");
            else
                img = QPixmap(":/fondo/fondovictoriaNivel2.png");
            escenaFinal->addPixmap(img);
            escenaFinal->setSceneRect(0, 0, 1200, 630);
            QString estilo =
                "QPushButton { background-color: transparent; border: none; }"
                "QPushButton:hover { background-color: rgba(255,255,255,40); border-radius:6px; }";
            if(nivelActual == 0)
            {
                QPushButton* btnSiguiente = new QPushButton();
                btnSiguiente->setFixedSize(180, 80);
                btnSiguiente->setCursor(Qt::PointingHandCursor);
                btnSiguiente->setStyleSheet(estilo);
                escenaFinal->addWidget(btnSiguiente)->setPos(720, 350);

                connect(btnSiguiente, &QPushButton::clicked, this, [this](){
                    mostrarSeleccionDificultad();
                });
            }
            else
            {
                QPushButton* btnMenu = new QPushButton();
                btnMenu->setFixedSize(180, 80);
                btnMenu->setCursor(Qt::PointingHandCursor);
                btnMenu->setStyleSheet(estilo);
                escenaFinal->addWidget(btnMenu)->setPos(720, 370);
                connect(btnMenu, &QPushButton::clicked, this, [this](){
                    estadoJuegoActual = EstadoJuego::Menu;
                    vista->setScene(escenaMenu);
                    ajustarVista();
                    musicaMenu->play();
                });
            }
            vista->setScene(escenaFinal);
            vista->setSceneRect(0, 0, 1200, 630);
            ajustarVista();
        });
        break;
    }
    case EstadoJuego::Derrota:
    {
        soloPintar = true;
        musicaNivel->stop();;
        QTimer::singleShot(2500, this, [this](){
            soloPintar = false;
            timer->stop();
            escenaJuego->clear();
            escenaDerrota->clear();
            QPixmap img(":/fondo/fondoDerrota.png");
            escenaDerrota->addPixmap(img);
            escenaDerrota->setSceneRect(0, 0, 1200, 630);
            QString estilo =
                "QPushButton { background-color: transparent; border: none; }"
                "QPushButton:hover { background-color: rgba(255,255,255,40); border-radius:6px; }";

            QPushButton* btnTryAgain = new QPushButton();
            btnTryAgain->setFixedSize(230, 80);
            btnTryAgain->setCursor(Qt::PointingHandCursor);
            btnTryAgain->setStyleSheet(estilo);
            escenaDerrota->addWidget(btnTryAgain)->setPos(380, 450);


            QPushButton* btnMenu = new QPushButton();
            btnMenu->setFixedSize(210, 80);
            btnMenu->setCursor(Qt::PointingHandCursor);
            btnMenu->setStyleSheet(estilo);
            escenaDerrota->addWidget(btnMenu)->setPos(650, 450);


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
    });
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


