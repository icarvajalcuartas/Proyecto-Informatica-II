#include "Personaje.h"
Personaje::Personaje(QString nombre, float posx, float posy,
                     unsigned short vida,QGraphicsItem* parent) :
    QGraphicsObject(parent),spriteActual(),frameActual(0),posx(posx),posy(posy),velx(0.0f),vely(0.0f), acelx(0.0f),
    acely(0.0f),ancho(0.0f),alto(0.0f),vida(vida),activo(true), nombre(nombre),
    estado(Estado::Quieto),zonaActual(ZonaAtaque::Inicial),dirActual(Direccion::Izquierda)
    {
    zonaMenLocal = QRectF(35, 5, 60, 25);
    zonaDoLocal = QRectF(30, 40, 65, 30);
    zonaKoteLocal = QRectF(25, 80, 70, 25);
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

    for (unsigned short int i=0; i<numFrames; ++i){
        qDebug() << "fila:" << fila;
        qDebug() << "numFrames:" << numFrames;
        qDebug() << "frameAncho:" << frameAncho;
        qDebug() << "frameAlto:" << frameAlto;
        QPixmap frame= hojaSprites.copy(i*frameAncho, fila*frameAlto, frameAncho, frameAlto);
        qDebug() << "Frame creado:"<< frame.width()<< frame.height();
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

QVector<QPixmap> *Personaje::obtenerSpriteActual() {
    AnimacionKey key{ estado, dirActual, zonaActual };

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
        spriteActual = frames->at(frameActual); update();
    }
}
void Personaje::actualizarSprite() {
    qDebug() << "actualizarSprite llamado";
    QVector<QPixmap>* frames = obtenerSpriteActual();
    if(frames && !frames->isEmpty()) {
        qDebug() << "frames encontrados:" << frames->size();
        frameActual = 0;
        spriteActual = frames->at(0);
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
    ZonaAtaque resultado = ZonaAtaque::Inicial;

    if(golpe.intersects(men)){
        resultado= ZonaAtaque::Men;
    }
    if(golpe.intersects(do_)){
        resultado= ZonaAtaque::Do;
    }
    if(golpe.intersects(kote)){
        resultado=ZonaAtaque::Kote;
    }
    return resultado;
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
QRectF Personaje::getZonaMen() const{return zonaMenLocal;}
QRectF Personaje::getZonaDo() const{return zonaDoLocal;}
QRectF Personaje::getZonaKote() const{return zonaKoteLocal;}
Personaje::~Personaje() {
    liberarSprites();
}




