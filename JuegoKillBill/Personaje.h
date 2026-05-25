#ifndef PERSONAJE_H
#define PERSONAJE_H
#include <QString>

class Personaje{
public:
    enum class ZonaAtaque {Men,Do,Kote};
    enum class Estado {Quieto,Moviendose,Atacando,Defendiendo,Saltando,
                        Vulnerable,Golpeado,Derrotado};
protected:
    float posx;
    float posy;
    float velx;
    float vely;
    float acelx;
    float acely;
    float ancho;
    float alto;
    unsigned short int vida;
    bool activo;
    QString nombre;
    Estado estado;
    ZonaAtaque zonaActual;
public:
    Personaje (QString nombre, float posx, float posy, unsigned short int vida);
    virtual void actualizar(float difTiempo);
    virtual void mover(float dirX, float dirY);
    virtual void iniciarAtaque(ZonaAtaque zona)=0;
    float getPosx() const;
    float getPosy() const;
    float getVelx() const;
    float getVely()const;
    float getAcelx() const;
    float getAcely() const;
    float getAncho() const;
    float getAlto() const;
    unsigned short int getVida() const;
    bool getActivo() const;
    QString getNombre() const;
    virtual ~Personaje()=default;
};

#endif // PERSONAJE_H
