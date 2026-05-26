#include "Bride.h"


Bride::Bride(float posx, float posy, unsigned short contZ):Personaje("The Bride",posx,posy,0),
    contZanshin(contZ),ataqueValido(false){}

Bride::Bride(float posx, float posy):Personaje("The Bride",posx,posy,0),contZanshin(USHRT_MAX){}

void Bride::realizarParry()
{
    estado=Estado::Defendiendo;
}

void Bride::input()
{

}

void Bride::actualizar()
{

}

void Bride::iniciarAtaque(ZonaAtaque zona)
{
    estado=Estado::Atacando;
}

void Bride::esAtaqueValido(ZonaAtaque zona)
{
    if(zonaActual==zona){
        ataqueValido=false;
        contZanshin=0;
    }else{
        ataqueValido=true;
        contZanshin+=1;
    }
}

unsigned short Bride::getContZanshin()
{
    return contZanshin;
}


