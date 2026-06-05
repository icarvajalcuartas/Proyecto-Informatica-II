#ifndef TEST_H
#define TEST_H
#include <QObject>
#include <QEvent>
#include <QKeyEvent>
#include "Bride.h"

class Test:public QObject
{
    Q_OBJECT
public:
    Bride *bride;
    Test(Bride *b):bride (b){}
protected:
    bool eventFilter(QObject* obj, QEvent* event) override
    {
        if(event->type() == QEvent::KeyPress)
        {
            QKeyEvent* key = static_cast<QKeyEvent*>(event);

            switch(key->key())
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
            }
        }
        else if(event->type() == QEvent::KeyRelease){
            QKeyEvent* key = static_cast<QKeyEvent*>(event);
            switch(key->key()){
            case Qt::Key_A:
                bride->detenerMovimiento();
                break;
            case Qt::Key_D:
                bride->detenerMovimiento();
                break;
            }
        }
        return QObject::eventFilter(obj, event);
    }
};

#endif // TEST_H
