#ifndef GPERSON_H
#define GPERSON_H
#include <QGraphicsRectItem>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include "dataperson.h"

class cScene;

class gPerson : public QGraphicsRectItem
{
    friend class dPerson;
public:
    gPerson(cScene *,  dPerson * dPerson);
    void SetY(int y) {ypos = y;}
    void Move(int x, int  y);
    float X(){return xpos;}
    float Y(){return ypos;}
    QString LastName() {return lastName;}


private:
    cScene * scene;
    dPerson * dataPerson;
    QGraphicsRectItem * box;
    QGraphicsSimpleTextItem * nameItem;
    float xpos;
    float ypos;
    float height;
    float width;
    QString firstName;
    QString lastName;
};



#endif // GPERSON_H
