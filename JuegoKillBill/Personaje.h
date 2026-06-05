#ifndef PERSONAJE_H
#define PERSONAJE_H
#include <QGraphicsObject>
#include <QString>
#include <QPixmap>
#include <QVector>
#include <QMap>

enum class ZonaAtaque {Men,Do,Kote,Inicial};
enum class Estado {Quieto,Moviendose,Atacando,Defendiendo,Saltando, Vulnerable,Golpeado,Derrotado};
enum class Direccion{Adelante, Atras, Derecha, Izquierda};

struct AnimacionKey {
    Estado estado;
    Direccion direccion;
    ZonaAtaque zona;
    bool operator<(const AnimacionKey& otra) const {
        if(static_cast<int>(estado) != static_cast<int>(otra.estado))
            return static_cast<int>(estado) < static_cast<int>(otra.estado);
        if(static_cast<int>(direccion) != static_cast<int>(otra.direccion))
            return static_cast<int>(direccion) < static_cast<int>(otra.direccion);
        return static_cast<int>(zona) < static_cast<int>(otra.zona); }
};

class Personaje: public QGraphicsObject{
    Q_OBJECT
protected:
    QPixmap spriteActual;
    QMap<AnimacionKey, QVector<QPixmap>*> sprites;
    unsigned short int frameActual;
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
    Direccion dirActual;
    QRectF zonaMenLocal;
    QRectF zonaDoLocal;
    QRectF zonaKoteLocal;
    QVector<QPixmap>* obtenerSpriteActual();
public:
    Personaje (QString nombre, float posx, float posy, unsigned short int vida,
              QGraphicsItem* parent = nullptr);
    QRectF boundingRect() const override = 0;
    void paint(QPainter* painter,const QStyleOptionGraphicsItem* option, QWidget* widget) override = 0;
    void cargarSprites(const QString& ruta, Estado estado, Direccion direccion,
                       ZonaAtaque zona, unsigned short int fila,
                       unsigned short int numFrames, unsigned short int frameAncho,unsigned short int frameAlto);
    void avanzarFrame();
    void actualizarSprite();
    void actualizarEscala25D(float yMin, float yMax,float escalaMin, float escalaMax);
    void liberarSprites();
    ZonaAtaque detectarZonaAtaque(QRectF &golpe);
    virtual void actualizar(float difTiempo)=0;
    virtual void iniciarAtaque(ZonaAtaque zona)=0;
    QRectF getHitboxAtaque() const;
    QRectF getHitboxCuerpo() const;
    virtual void recibirGolpe()=0;
    void seccionarSpritesheet(const QString& ruta);
    void setVelx(float vx);
    void setVely(float vy);
    void setAcelx(float ax);
    void setAcely(float ay);
    void setPosx(float newPosx);
    void setPosy(float newPosy);
    void setFrameActual(unsigned short int frame);
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
    ZonaAtaque getZonaActual() const;
    QRectF getZonaMen() const;
    QRectF getZonaDo() const;
    QRectF getZonaKote() const;
    Estado getEstado() const;
    virtual ~Personaje();


};
#endif // PERSONAJE_H
