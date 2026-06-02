#include "Bride.h"


Bride::Bride(float posx, float posy):Personaje("The Bride",posx,posy,5),
    contZanshin(0),puntaje(0),ataqueValido(false),fisicaActual(ModoFisica::Uniforme),
    enSuelo(true), dirActual(Direccion::Ninguna){}

void Bride::setModoFisica(ModoFisica modo)
{
    fisicaActual=modo;
}

void Bride::setDireccion(Direccion dir)
{
    dirActual = dir;
    if(dir == Direccion::Izquierda){
        velx = -FUERZA_HORIZONTAL;
    }
    else if(dir == Direccion::Derecha) {
        velx =  FUERZA_HORIZONTAL;
    }
    else{
        velx =  0;
    }
}

void Bride::setEnSuelo(bool suelo)
{
    enSuelo = suelo;
}


void Bride::realizarParry()
{
    estado=Estado::Defendiendo;
}

void Bride::input()
{

}

void Bride::actualizar(float difTiempo)
{
    switch(fisicaActual){
        case (ModoFisica::Uniforme):{
            movRectilineo(difTiempo);
            break;
        }
        case (ModoFisica::Parabolica):{
            movParabolico(difTiempo);
            break;
        }
        case (ModoFisica::dos5Dimensiones):{
            movDosDimensiones(difTiempo);
            break;
        }
        case (ModoFisica::SaltoY):{
            movSaltoY(difTiempo);
            break;
        }
    }
}

void Bride::iniciarAtaque(ZonaAtaque zona)
{
    zonaActual=zona;
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

void Bride::movRectilineo(float difTiempo)
{
    posx += velx * difTiempo;
}

void Bride::saltar()
{
    if(enSuelo){
        vely = -FUERZA_SALTO;
        velx=0;
        estado = Estado::Saltando;
        fisicaActual = ModoFisica::SaltoY;
        enSuelo=false;
    }
}

void Bride::movSaltoY(float difTiempo)
{
    vely += acely * difTiempo;
    posy += vely  * difTiempo;
}

void Bride::movParabolico(float difTiempo)
{
    vely += acely * difTiempo;
    posx += velx * difTiempo;
    posy += vely * difTiempo;
}

void Bride::movDosDimensiones(float difTiempo)
{
    velx += acelx * difTiempo;
    vely += acely * difTiempo;
    posx += velx * difTiempo;
    posy += vely * difTiempo;
}


bool Bride::getEnSuelo() const
{
    return enSuelo;
}

unsigned short Bride::getContZanshin() const
{
    return contZanshin;
}


