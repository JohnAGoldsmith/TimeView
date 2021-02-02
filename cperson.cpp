#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QGraphicsScene>
#include <QStylePainter>
#include <QGradient>
#include "cperson.h"
#include "cscene.h"


cPerson::cPerson(cScene * thisScene, QString FirstName, QString LastName, int x, int y)
{
   xpos = x;
   ypos = y;
   width = 100;
   height = 40;
   firstName = FirstName;
   lastName = LastName;
   QString name = firstName + " "  + lastName;





   box = new QGraphicsRectItem(QRect(xpos, ypos, width, height));
   box->setBrush(QColor(00,190,200,255));
   thisScene->addItem(box);
   nameItem = thisScene->addSimpleText(name);
   nameItem->setPos(xpos,ypos);
}

void cPerson::move(int x, int y){
   box->moveBy(x,y);
   nameItem->moveBy(x,y);
}
