#include "Nivel.h"
#include <QRandomGenerator>

Nivel::Nivel(unsigned short int numero, QGraphicsScene *escena): numeroNivel(numero),
    escena(escena), bride(nullptr), oren(nullptr),terminado(false),textoPuntosBride(nullptr), textoPuntosOren(nullptr),
    textoZanshin(nullptr),sonidoGolpe(nullptr), sonidoGritoBride(nullptr),
    bannerZanshin(nullptr), textoBanner(nullptr)
    {}


void Nivel::cargar()
{

    QString rutaFondo;
    switch(numeroNivel)
    {
    case 1:
        rutaFondo = ":/fondo/fondonivel1.png";
        break;
    case 2:
        rutaFondo = ":/fondo/fondonivel2.png";
        break;
    default:
        rutaFondo = ":/fondo/fondonivel1.png";
        break;
    }

    QPixmap fondoNivel(rutaFondo);
    qDebug() << "Fondo nivel" << numeroNivel << "cargado:" << !fondoNivel.isNull();
    QGraphicsPixmapItem* itemFondo = escena->addPixmap(fondoNivel);
    itemFondo->setZValue(-1);
    bride = new Bride(POS_X_BRIDE,POS_Y_PERSONAJES);
    oren = new Oren(POS_X_OREN,POS_Y_PERSONAJES);
    escena->addItem(bride);
    escena->addItem(oren);

    QFont marcadorFuente("Arial", 18, QFont::Bold);


    textoPuntosBride = escena->addText("THE BRIDE: 0", marcadorFuente);
    textoPuntosBride->setDefaultTextColor(Qt::yellow);
    textoPuntosBride->setPos(980, 10);
    textoPuntosBride->setZValue(10);


    textoZanshin = escena->addText("ZANSHIN: 0", marcadorFuente);
    textoZanshin->setDefaultTextColor(Qt::cyan);
    textoZanshin->setPos(980, 40);
    textoZanshin->setZValue(10);


    textoPuntosOren = escena->addText("O-REN: 0", marcadorFuente);
    textoPuntosOren->setDefaultTextColor(Qt::red);
    textoPuntosOren->setPos(10, 10);
    textoPuntosOren->setZValue(10);

    connect(bride, &Bride::zanshinActualizado, this, [this](unsigned short c){
        if(textoZanshin)
            textoZanshin->setPlainText("ZANSHIN: " + QString::number(c));
    });
    connect(bride, &Bride::zanshinEspecialIniciado, this, [this](){
        if(textoZanshin)
            textoZanshin->setDefaultTextColor(Qt::green);
    });

    connect(bride, &Bride::zanshinEspecialterminado, this, [this](){
        if(textoZanshin)
            textoZanshin->setDefaultTextColor(Qt::cyan);
    });

    connect(bride, &Bride::zanshinEspecialIniciado,
            oren, &Oren::activarModoDebilitado);

    connect(bride, &Bride::zanshinEspecialterminado,
            oren, &Oren::desactivarModoDebilitado);

    sonidoGolpe = new QSoundEffect(this);
    sonidoGolpe->setSource(QUrl("qrc:/audio/espadaGolpe.wav"));
    sonidoGolpe->setVolume(0.7f);

    sonidoGritoBride = new QSoundEffect(this);
    sonidoGritoBride->setVolume(0.7f);

    bannerZanshin = new QGraphicsRectItem(300, 15, 600, 70);
    bannerZanshin->setBrush(QBrush(QColor(180, 0, 0, 200)));
    bannerZanshin->setPen(QPen(Qt::yellow, 2));
    bannerZanshin->setZValue(20);
    bannerZanshin->setVisible(false);
    escena->addItem(bannerZanshin);

    QFont fuenteBanner("Helvetica", 22, QFont::Bold);
    fuenteBanner.setItalic(true);
    textoBanner = escena->addText(" ZANSHIN ESPECIAL ", fuenteBanner);
    textoBanner->setDefaultTextColor(Qt::yellow);
    textoBanner->setPos(340, 28);
    textoBanner->setZValue(21);
    textoBanner->setVisible(false);

    connect(bride, &Bride::zanshinEspecialIniciado, this, [this](){
    bannerZanshin->setVisible(true);
    textoBanner->setVisible(true);
    });

    connect(bride, &Bride::zanshinEspecialterminado, this, [this](){
    bannerZanshin->setVisible(false);
    textoBanner->setVisible(false);
    });
}

void Nivel::verificarBordes()
{
    if(!bride || !oren) return;
    if(bride->getPosx() < BORDE_IZQUIERDO)
    {
        bride->setPosx(BORDE_IZQUIERDO);
        if(bride->getEstado() != Estado::Saltando)
            bride->setVelx(0);
    }
    if(bride->getPosx() > BORDE_DERECHO)
    {
        bride->setPosx(BORDE_DERECHO);
        if(bride->getEstado() != Estado::Saltando)
            bride->setVelx(0);
    }

    if(oren->getPosx() < BORDE_IZQUIERDO)
    {
        oren->setPosx(BORDE_IZQUIERDO);
        oren->setVelx(0);
    }
    if(oren->getPosx() > BORDE_DERECHO)
    {
        oren->setPosx(BORDE_DERECHO);
        oren->setVelx(0);
    }
}

void Nivel::inputJugador(QKeyEvent *evento)
{
    if(!bride)
        return;

    teclasActivas.insert(evento->key());
    if(bride->getEstado() == Estado::Saltando) return;
    if(bride->getZanshinEspecialActivo())
    {
        if(teclasActivas.contains(Qt::Key_Space) &&
            teclasActivas.contains(Qt::Key_A))
        {
            bride->saltarIzquierda();
            return;
        }
        if(teclasActivas.contains(Qt::Key_Space) &&
            teclasActivas.contains(Qt::Key_D))
        {
            bride->saltarDerecha();
            return;
        }

        if(evento->key() == Qt::Key_Space)
        {
            bride->saltar();
            return;
        }
    }
    switch(evento->key())
    {
    case Qt::Key_A:
        bride->moverIzquierda();
        break;

    case Qt::Key_D:
        bride->moverDerecha();
        break;

    case Qt::Key_J:
        bride->ataqueMen();
        break;

    case Qt::Key_K:
        bride->ataqueDo();
        break;

    case Qt::Key_L:
        bride->ataqueKote();
        break;
    }
}

void Nivel::inputJugadorLiberada(QKeyEvent *evento)
{
    if(!bride)
        return;
    teclasActivas.remove(evento->key());
    if(bride->getEstado() == Estado::Saltando) return;
    switch(evento->key())
    {
    case Qt::Key_A:
    case Qt::Key_D:
        bride->detenerMovimiento();
        break;
    }
}

void Nivel::actualizar(float dt)
{

    if(bride) bride->actualizar(dt);
    if(oren) oren->actualizar(dt);

    verificarBordes();
    verificarSolapamiento();
    verificarColisiones();

}
void Nivel::verificarColisiones()
{
    if(!bride || !oren)
        return;

    QRectF ataqueBride = bride->mapToScene(bride->getHitboxAtaque()).boundingRect();
    QRectF bodyOren = oren->mapToScene(oren->getHitboxCuerpo()).boundingRect();


    QRectF ataqueOren = oren->mapToScene(oren->getHitboxAtaque()).boundingRect();
    QRectF bodyBride = bride->mapToScene(bride->getHitboxCuerpo()).boundingRect();


    if(bride->getEstado() == Estado::Atacando && ataqueBride.intersects(bodyOren))
    {
        if(!colisionAtaqueBride){
            colisionAtaqueBride = true;
            puntosBride++;
            oren->recibirGolpe();
            bride->registrarZanshin(bride->getZonaActual());
            actualizarMarcador();
            sonidoGolpe->play();
        }
    }
    else{
        colisionAtaqueBride = false;
    }


    if(bride->getEstado()!= Estado::Saltando && oren->getEstado() == Estado::Atacando &&
        ataqueOren.intersects(bodyBride))
    {
        if(!colisionAtaqueOren){
            colisionAtaqueOren = true;
            puntosOren++;
            bride->recibirGolpe();
            actualizarMarcador();
            int grito = QRandomGenerator::global()->bounded(2);
            sonidoGritoBride->setSource(
                QUrl(grito == 0 ? "qrc:/audio/grito1.wav"
                                : "qrc:/audio/grito2.wav")
                );
            sonidoGritoBride->play();
        }
    }
    else{
        colisionAtaqueOren = false;
    }

    if(puntosBride >= MAX_PUNTOS)
    {
        emit victoria();
        return;
    }

    if(puntosOren >= MAX_PUNTOS)
    {
        emit derrota();
        return;
    }
}

void Nivel::actualizarMarcador()
{
    if(textoPuntosBride)
        textoPuntosBride->setPlainText("THE BRIDE: " + QString::number(puntosBride));
    if(textoPuntosOren)
        textoPuntosOren->setPlainText("O-REN: " + QString::number(puntosOren));
}

void Nivel::verificarSolapamiento()
{
    if(!bride || !oren) return;
    if(bride->getEstado() == Estado::Saltando) return;
    float dx = bride->getPosx() - oren->getPosx();
    float distancia = std::abs(dx);

    if(distancia < DISTANCIA_MINIMA)
    {
        if(dx < 0)
        {
            bride->setPosx(oren->getPosx() - DISTANCIA_MINIMA);
        }
        else
        {
            bride->setPosx(oren->getPosx() + DISTANCIA_MINIMA);
        }
        bride->setVelx(0);
    }
}


