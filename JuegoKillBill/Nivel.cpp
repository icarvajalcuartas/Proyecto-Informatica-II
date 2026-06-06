#include "Nivel.h"


Nivel::Nivel(unsigned short int numero, QGraphicsScene *escena): numeroNivel(numero),
    escena(escena), bride(nullptr), oren(nullptr),terminado(false),textoPuntosBride(nullptr), textoPuntosOren(nullptr),
    textoZanshin(nullptr){}


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
    bride = new Bride(600,300);
    oren = new Oren(300,300);
    escena->addItem(bride);
    escena->addItem(oren);

    QFont marcadorFuente("Arial", 18, QFont::Bold);
    // connect(bride, &Bride::zanshinActualizado,this, [](unsigned short c){
    //             qDebug() << "Zanshin:" << c;
    // });

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

}

void Nivel::verificarBordes()
{
    if(bride->getPosx() < BORDE_IZQUIERDO)
    {
        bride->setPosx(BORDE_IZQUIERDO);
        bride->setVelx(0);
    }
    if(bride->getPosx() > BORDE_DERECHO)
    {
        bride->setPosx(BORDE_DERECHO);
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

    switch(evento->key())
    {
    case Qt::Key_A:
        bride->moverIzquierda();
        break;

    case Qt::Key_D:
        bride->moverDerecha();
        break;

    case Qt::Key_Space:
        bride->saltar();
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


