#include "Personaje.h"
#include <QPainter>
Personaje::Personaje(QString nombre, float posx, float posy,
                     unsigned short vida,QGraphicsItem* parent) :
    QGraphicsObject(parent),spriteActual(),frameActual(0),posx(posx),posy(posy),velx(0.0f),vely(0.0f), acelx(0.0f),
    acely(0.0f),ancho(0.0f),alto(0.0f),vida(vida),activo(true), nombre(nombre),
    estado(Estado::Quieto),zonaActual(ZonaAtaque::Inicial),dirActual(Direccion::Izquierda)
    {
    setPos(posx,posy);
    zonaMenLocal = QRectF(40, 10, 40, 20);
    zonaDoLocal = QRectF(20, 40, 80, 50);
    zonaKoteLocal = QRectF(30, 100, 70, 20);
    }

void Personaje:: cargarSprites(const QString& ruta, Estado estado, Direccion direccion,
                              ZonaAtaque zona, unsigned short int fila,
                              unsigned short int numFrames, unsigned short int frameAncho,
                              unsigned short int frameAlto){
    QPixmap hojaSprites (ruta);
    if(hojaSprites.isNull()){
        qDebug()<<"No se encontro la ruta";
        return;
    }else{
        qDebug()<<" se encontro la ruta";
    }
    qDebug() << hojaSprites.width()
             << hojaSprites.height();
    QVector<QPixmap>* frames = new QVector<QPixmap>();
    frames->reserve(numFrames);
    qDebug()
        << "Fila:" << fila
        << "Estado:" << static_cast<int>(estado)
        << "Direccion:" << static_cast<int>(direccion)
        << "Zona:" << static_cast<int>(zona);

    for (unsigned short int i=0; i<numFrames; ++i){
        QPixmap frame= hojaSprites.copy(i*frameAncho, fila*frameAlto, frameAncho, frameAlto);
        frames->append(frame);
    }

    AnimacionKey key{ estado, direccion, zona };

    if(sprites.contains(key))
    {
        delete sprites[key];
    }
    qDebug() << "Frames cargados:" << frames->size();
    sprites.insert(key,frames);
}

Estado Personaje::getEstado() const
{
    return estado;
}

QVector<QPixmap> *Personaje::obtenerSpriteActual() {
    AnimacionKey key{ estado, dirActual, zonaActual };
    qDebug()
        << "Buscando:"
        << "Estado" << static_cast<int>(estado)
        << "Direccion" << static_cast<int>(dirActual)
        << "Zona" << static_cast<int>(zonaActual);
    auto it = sprites.find(key);
    if(it != sprites.end()){
        qDebug() << "Animacion encontrada";
        return it.value();
    }
    qDebug() << "Animacion NO encontrada";
    return nullptr;
}

void Personaje::avanzarFrame() {
    QVector<QPixmap>* frames = obtenerSpriteActual();

    if(frames && !frames->isEmpty()){
        frameActual = (frameActual + 1) % frames->size();
    }
}
void Personaje::actualizarSprite() {
    qDebug() << "actualizarSprite llamado";
    QVector<QPixmap>* frames = obtenerSpriteActual();
    if(frames && !frames->isEmpty()) {
        if(frameActual >= frames->size()){
            frameActual = 0;
        }
        qDebug() << "frames encontrados:" << frames->size();
        spriteActual = frames->at(frameActual);
        qDebug() << spriteActual.width()<< spriteActual.height();
        update();
    }
}
void Personaje::actualizarEscala25D(float yMin, float yMax, float escalaMin, float escalaMax) {
    float t = (posy - yMin) / (yMax - yMin);
    float escala = escalaMin + t * (escalaMax - escalaMin);
    setScale(escala);
}
void Personaje::liberarSprites() {
    qDeleteAll(sprites);
    sprites.clear();
}

ZonaAtaque Personaje::detectarZonaAtaque(QRectF &golpe)
{
    QRectF men = mapRectToScene(getZonaMen());
    QRectF do_=mapRectToScene(getZonaDo());
    QRectF kote= mapRectToScene(getZonaKote());

    if(golpe.intersects(men)){
        return ZonaAtaque::Men;
    }
    if(golpe.intersects(do_)){
        return ZonaAtaque::Do;
    }
    if(golpe.intersects(kote)){
        return ZonaAtaque::Kote;
    }
    return  ZonaAtaque::Inicial;
}

QRectF Personaje::getHitboxAtaque() const
{
    if(estado != Estado::Atacando)
        return QRectF();

    QRectF hitbox;
    switch(zonaActual)
    {
    case ZonaAtaque::Men:
        hitbox = QRectF(0, 0, 30, 40);
        break;

    case ZonaAtaque::Do:
        hitbox = QRectF(0, 40, 30, 40);
        break;

    case ZonaAtaque::Kote:
        hitbox = QRectF(0, 80, 30, 40);
        break;

    default:
        return QRectF();
    }
    float offsetX = (dirActual == Direccion::Derecha)? getHitboxCuerpo().width()
                        : -hitbox.width();

    hitbox.translate(offsetX, 0);
    return hitbox;
}

QRectF Personaje:: getHitboxCuerpo() const{
    return QRectF(10,10,90,100);
}
void Personaje::seccionarSpritesheet(const QString &ruta)
{
    unsigned short int filasSprites = 32;

    Direccion direcciones[4] ={Direccion::Adelante,Direccion::Atras,Direccion::Derecha,Direccion::Izquierda};

    for(unsigned short int i = 0; i < filasSprites; i++){
        Estado estadoSprite;
        ZonaAtaque zonaSprite;
        Direccion direccionSprite;
        direccionSprite = direcciones[i % 4];
        switch(i/4){
        case 0:
            estadoSprite = Estado::Quieto;
            zonaSprite = ZonaAtaque::Inicial;
            break;
        case 1:
            estadoSprite = Estado::Moviendose;
            zonaSprite = ZonaAtaque::Inicial;
            break;
        case 2:
            estadoSprite = Estado::Atacando;
            zonaSprite = ZonaAtaque::Men;
            break;
        case 3:
            estadoSprite = Estado::Atacando;
            zonaSprite = ZonaAtaque::Do;
            break;
        case 4:
            estadoSprite = Estado::Atacando;
            zonaSprite = ZonaAtaque::Kote;
            break;
        case 5:
            estadoSprite = Estado::Saltando;
            zonaSprite = ZonaAtaque::Inicial;
            break;
        case 6:
            estadoSprite = Estado::Golpeado;
            zonaSprite = ZonaAtaque::Inicial;
            break;
        case 7:
            estadoSprite = Estado::Derrotado;
            zonaSprite = ZonaAtaque::Inicial;
            break;
        default:
            continue;
        }
        cargarSprites(ruta,estadoSprite,direccionSprite,zonaSprite,i,2,128,128);
        if (estadoSprite == Estado::Atacando){
            cargarSprites(ruta,Estado::Defendiendo,direccionSprite,zonaSprite,i,2,128,128);
        }
    }
}

void Personaje::setVelx(float vx){velx = vx;}
void Personaje::setVely(float vy){vely = vy;}
void Personaje::setAcelx(float ax){acelx = ax;}
void Personaje::setAcely(float ay){acely = ay;}
void Personaje::setPosx(float newPosx){posx = newPosx;}
void Personaje::setPosy(float newPosy){posy = newPosy;}
void Personaje::setFrameActual(unsigned short frame) {

    QVector<QPixmap>* frames = obtenerSpriteActual();
    if(frames && frame < frames->size()) {
        frameActual = frame; spriteActual = frames->at(frame); update();
    }
}
float Personaje::getPosx() const {return posx;}
float Personaje::getPosy() const{return posy;}
float Personaje::getVelx() const{return velx;}
float Personaje::getVely() const{return vely;}
float Personaje::getAcelx() const{return acelx;}
float Personaje::getAcely() const{return acely;}
float Personaje::getAncho() const{return ancho;}
float Personaje::getAlto() const{return alto;}
unsigned short Personaje::getVida() const{return vida;}
bool Personaje::getActivo() const{return activo;}
QString Personaje::getNombre() const{return nombre;}
ZonaAtaque Personaje::getZonaActual() const{return zonaActual;}
QRectF Personaje::getZonaMen() const{return zonaMenLocal;}
QRectF Personaje::getZonaDo() const{return zonaDoLocal;}
QRectF Personaje::getZonaKote() const{return zonaKoteLocal;}
Personaje::~Personaje() {
    liberarSprites();
}





