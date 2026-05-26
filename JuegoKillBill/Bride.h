#ifndef BRIDE_H
#define BRIDE_H
#include "Personaje.h"
class Bride:public Personaje{
private:
    unsigned short int contZanshin;
    bool ataqueValido;
public:
    Bride(float posx,float posy, unsigned short int contZ);
    Bride(float posx,float posy);
    void realizarParry();
    void input();
    void actualizar();
    void iniciarAtaque(ZonaAtaque zona)override;
    void esAtaqueValido(ZonaAtaque zona);
    unsigned short int getContZanshin();


};

#endif // BRIDE_H
