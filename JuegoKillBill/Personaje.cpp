#include "Personaje.h"
Personaje::Personaje(QString nombre, float posx, float posy,
                     unsigned short vida,QGraphicsItem* parent) :
    QGraphicsObject(parent),spriteActual(),frameActual(0),posx(posx),posy(posy),velx(0.0f),vely(0.0f), acelx(0.0f),
    acely(0.0f),ancho(0.0f),alto(0.0f),vida(vida),activo(true), nombre(nombre),
    estado(Estado::Quieto),zonaActual(ZonaAtaque::Inicial),dirActual(Direccion::Izquierda)
    {}

void Personaje:: cargarSprites(const QString& ruta, Estado estado, Direccion direccion,
                              ZonaAtaque zona, unsigned short int fila,
                              unsigned short int numFrames, unsigned short int frameAncho,
                              unsigned short int frameAlto){
    QPixmap hojaSprites (ruta);
    if(hojaSprites.isNull()){
        return;
    }
    QVector<QPixmap>* frames = new QVector<QPixmap>();
    frames->reserve(numFrames);

    for (unsigned short int i=0; i<numFrames; ++i){
        frames->append(hojaSprites.copy(i*frameAncho, fila*frameAlto, frameAncho, frameAlto));
    }

    AnimacionKey key{ estado, direccion, zona };

    if(sprites.contains(key))
    {
        delete sprites[key];
    }
    sprites.insert(key,frames);
}

QVector<QPixmap> *Personaje::obtenerSpriteActual() {
    AnimacionKey key{ estado, dirActual, zonaActual };

    auto it = sprites.find(key);
    if(it != sprites.end())
        return it.value();

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
    QVector<QPixmap>* frames = obtenerSpriteActual();
    if(frames && !frames->isEmpty()) {
        frameActual = 0;
        spriteActual = frames->at(0);
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
Personaje::~Personaje() {
    liberarSprites();
}




