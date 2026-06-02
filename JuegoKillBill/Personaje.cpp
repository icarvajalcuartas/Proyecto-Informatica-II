#include "Personaje.h"



Personaje::Personaje(QString nombre, float posx, float posy, unsigned short vida)
    : posx(posx),posy(posy),velx(0.0f),vely(0.0f),acelx(0.0f),acely(0.0f),
    ancho(0.0f),alto(0.0f),vida(vida),activo(true),nombre(nombre),
    estado(Estado::Quieto),zonaActual(ZonaAtaque::Inicial){}



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




