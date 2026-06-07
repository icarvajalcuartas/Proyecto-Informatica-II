#include "Nivel.h"
#include <QRandomGenerator>

Nivel::Nivel(unsigned short int numero, QGraphicsScene *escena): numeroNivel(numero),
    escena(escena), bride(nullptr), oren(nullptr),bill(nullptr),puntosBride (0),puntosOren (0),
    terminado(false),colisionAtaqueBride (false),colisionAtaqueOren (false),
    colisionAtaqueBill (false),parryExitosoActivo (false),dificultadBill (Dificultad::Aprendiz),
    acelerandoActivo (false),frenandoActivo (false),
    textoPuntosBride(nullptr), textoPuntosOren(nullptr),
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
    if(numeroNivel == 1) cargarNivel1();
    else cargarNivel2();
}

void Nivel::cargarNivel1()
{
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
    textoZanshin->setDefaultTextColor(Qt::darkMagenta);
    textoZanshin->setPos(980, 40);
    textoZanshin->setZValue(10);


    textoPuntosOren = escena->addText("O-REN: 0", marcadorFuente);
    textoPuntosOren->setDefaultTextColor(Qt::red);
    textoPuntosOren->setPos(10, 10);
    textoPuntosOren->setZValue(10);

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

    connect(bride, &Bride::zanshinActualizado, this, [this](unsigned short c){
        if(textoZanshin) textoZanshin->setPlainText("ZANSHIN: " + QString::number(c));
    });
    connect(bride, &Bride::zanshinEspecialIniciado, this, [this](){
        if(textoZanshin) textoZanshin->setDefaultTextColor(Qt::green);
        if(bannerZanshin) bannerZanshin->setVisible(true);
        if(textoBanner)   textoBanner->setVisible(true);
    });
    connect(bride, &Bride::zanshinEspecialterminado, this, [this](){
        if(textoZanshin) textoZanshin->setDefaultTextColor(Qt::darkMagenta);
        if(bannerZanshin) bannerZanshin->setVisible(false);
        if(textoBanner)   textoBanner->setVisible(false);
    });
    connect(bride, &Bride::zanshinEspecialIniciado, oren,  &Oren::activarModoDebilitado);
    connect(bride, &Bride::zanshinEspecialterminado, oren,  &Oren::desactivarModoDebilitado);

}

void Nivel::cargarNivel2()
{
    bride = new Bride(POS_X_BRIDE, POS_Y_PERSONAJES);
    bill  = new Bill(POS_X_BILL,  POS_Y_PERSONAJES, dificultadBill);
    escena->addItem(bride);
    escena->addItem(bill);

    QFont fuente("Helvetica", 16, QFont::Bold);
    QGraphicsTextItem* textoInfo = escena->addText("UN GOLPE DECIDE TODO", fuente);
    textoInfo->setDefaultTextColor(Qt::yellow);
    textoInfo->setPos(450, 10);
    textoInfo->setZValue(10);

    connect(bill, &Bill::entroEnVulnerabilidad, this, [this](){
    });
    sonidoGolpe = new QSoundEffect(this);
    sonidoGolpe->setSource(QUrl("qrc:/audio/espadaGolpe.wav"));
    sonidoGolpe->setVolume(0.7f);

    sonidoGritoBride = new QSoundEffect(this);
    sonidoGritoBride->setVolume(0.7f);
}

void Nivel::verificarBordes()
{
    if(!bride) return;
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

    if(numeroNivel == 2)
    {
        if(bride->getPosy() < BORDE_ARRIBA)
        {
            bride->setPosy(BORDE_ARRIBA);
            bride->setVely(0);
        }
        if(bride->getPosy() > BORDE_ABAJO)
        {
            bride->setPosy(BORDE_ABAJO);
            bride->setVely(0);
        }

        if(bill)
        {
            if(bill->getPosx() < BORDE_IZQUIERDO){
                bill->setPosx(BORDE_IZQUIERDO);
                bill->setVelx(0);
            }
            if(bill->getPosx() > BORDE_DERECHO){
                bill->setPosx(BORDE_DERECHO);
                bill->setVelx(0);
            }
            if(bill->getPosy() < BORDE_ARRIBA){
                bill->setPosy(BORDE_ARRIBA);
                bill->setVely(0);
            }
            if(bill->getPosy() > BORDE_ABAJO){
                bill->setPosy(BORDE_ABAJO);
                bill->setVely(0);
            }
        }
    }
    else if(oren) {
        if(oren->getPosx() < BORDE_IZQUIERDO){
            oren->setPosx(BORDE_IZQUIERDO);
            oren->setVelx(0);
        }
        if(oren->getPosx() > BORDE_DERECHO){
            oren->setPosx(BORDE_DERECHO);
            oren->setVelx(0);
        }
    }

}

void Nivel::inputJugador(QKeyEvent *evento)
{
    if(!bride)
        return;

    teclasActivas.insert(evento->key());
    if(numeroNivel == 2)
    {
        if(teclasActivas.contains(Qt::Key_R))
        {
            if(evento->key() == Qt::Key_J)
            {
                bride->activarParry(ZonaAtaque::Men);
                return;
            }
            if(evento->key() == Qt::Key_K)
            {
                bride->activarParry(ZonaAtaque::Do);
                return;
            }
            if(evento->key() == Qt::Key_L)
            {
                bride->activarParry(ZonaAtaque::Kote);
                return;
            }
        }

        switch(evento->key())
        {
        case Qt::Key_A: bride->moverIzquierdaXY(); break;
        case Qt::Key_D: bride->moverDerechaXY();   break;
        case Qt::Key_W: bride->moverAtras();  break;
        case Qt::Key_S: bride->moverAdelante(); break;
        case Qt::Key_Q: acelerandoActivo = true; break;
        case Qt::Key_E: frenandoActivo = true; break;
        case Qt::Key_J: bride->ataqueMen(); break;
        case Qt::Key_K: bride->ataqueDo(); break;
        case Qt::Key_L: bride->ataqueKote(); break;
        }
        return;
    }

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
    if(numeroNivel == 2)
    {
        switch(evento->key())
        {
        case Qt::Key_A:
        case Qt::Key_D:
            bride->setAcelx(0);
            bride->detenerMovimiento();
            break;
        case Qt::Key_W:
        case Qt::Key_S:
            bride->detenerEjeY();
            break;
        case Qt::Key_R:
            bride->desactivarParry();
            break;
        case Qt::Key_Q: acelerandoActivo = false; break;
        case Qt::Key_E: frenandoActivo = false; break;
        }
        return;
    }
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

    if(numeroNivel == 2 && bride)
    {
        if(acelerandoActivo) bride->acelerar();
        if(frenandoActivo)   bride->frenar();
    }
    if(bride) bride->actualizar(dt);

    if(numeroNivel == 1 && oren)
        oren->actualizar(dt);
    else if(numeroNivel == 2 && bill)
        bill->actualizar(dt);

    verificarBordes();

    if(numeroNivel == 1)
    {
        verificarSolapamiento();
        verificarColisiones();
    }
    else if(numeroNivel == 2)
    {
        actualizarEscala();
        verificarColisionesNivel2();
    }

}

bool Nivel::getTerminado() const{return terminado;}
unsigned short int Nivel::getNumeroNivel()  const{return numeroNivel;}
Bride* Nivel::getBride() const{return bride;}

void Nivel::setDificultad(Dificultad dif)
{
    dificultadBill = dif;
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
            if(!sonidoGolpe->isPlaying()) sonidoGolpe->play();

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
            if(!sonidoGritoBride->isPlaying()) sonidoGritoBride->play();
        }
    }
    else{
        colisionAtaqueOren = false;
    }

    if(puntosBride >= MAX_PUNTOS)
    {
        oren->setEstado(Estado::Derrotado);
        bride->update();
        emit victoria();
        return;
    }

    if(puntosOren >= MAX_PUNTOS)
    {
        bride->setEstado(Estado::Derrotado);
        oren->update();
        emit derrota();
        return;
    }
}

void Nivel::verificarColisionesNivel2()
{
    if(!bride || !bill) return;

    QRectF ataqueBride = bride->mapToScene(bride->getHitboxAtaque()).boundingRect();
    QRectF bodyBill    = bill->mapToScene(bill->getHitboxCuerpo()).boundingRect();
    QRectF ataqueBill  = bill->mapToScene(bill->getHitboxAtaque()).boundingRect();
    QRectF bodyBride   = bride->mapToScene(bride->getHitboxCuerpo()).boundingRect();


    if(bride->getEstado() == Estado::Atacando && ataqueBride.intersects(bodyBill))
    {
        if(!colisionAtaqueBride)
        {
            colisionAtaqueBride = true;

            if(bill->getEstado() == Estado::Vulnerable ||
                bill->getEstado() == Estado::Quieto     ||
                bill->getEstado() == Estado::Moviendose)
            {
                bill->recibirGolpe();
                bill->registrarAtaqueBride(bride->getZonaActual());
                qDebug() << "[Nivel2] registrarAtaqueBride llamado con zona:"
                         << static_cast<int>(bride->getZonaActual());
                if(sonidoGolpe && !sonidoGolpe->isPlaying())
                    sonidoGolpe->play();
                bill->setEstado(Estado::Derrotado);
                bill->update();
                emit victoria();
                return;
            }
        }
    }
    else colisionAtaqueBride = false;

    if(bill->getEstado() == Estado::Atacando && ataqueBill.intersects(bodyBride))
    {
        if(!colisionAtaqueBill)
        {
            colisionAtaqueBill = true;

            if(bride->getEnModoParry() &&
                bride->getZonaParry() == bill->getZonaActual() &&
                bride->getTiempoParry() <= bill->getVentanaParry())
            {
                bill->registrarParryBride(bill->getZonaActual());
                bill->activarVulnerabilidad();
                parryExitosoActivo = true;
            }
            else
            {
                bill->registrarGolpeExitoso(bill->getZonaActual());
                bride->recibirGolpe();
                int grito = QRandomGenerator::global()->bounded(2);
                sonidoGritoBride->setSource(
                    QUrl(grito == 0 ? "qrc:/audio/grito1.wav"
                                    : "qrc:/audio/grito2.wav"));
                if(!sonidoGritoBride->isPlaying())
                    sonidoGritoBride->play();
                bride->setEstado(Estado::Derrotado);
                bride->update();
                emit derrota();
                return;
            }
        }
    }
    else colisionAtaqueBill = false;

}

void Nivel::actualizarMarcador()
{
    if(textoPuntosBride)
        textoPuntosBride->setPlainText("THE BRIDE: " + QString::number(puntosBride));
    if(textoPuntosOren)
        textoPuntosOren->setPlainText("O-REN: " + QString::number(puntosOren));
}

void Nivel::actualizarEscala()
{
    if(bride) bride->actualizarEscala25D(Y_MIN_ESCALA, Y_MAX_ESCALA, ESCALA_MIN, ESCALA_MAX);
    if(bill)  bill->actualizarEscala25D(Y_MIN_ESCALA, Y_MAX_ESCALA, ESCALA_MIN, ESCALA_MAX);
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


