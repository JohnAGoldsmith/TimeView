#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QGraphicsScene>
#include <QStylePainter>
#include <QGradient>
#include <QDebug>
#include <QColor>
#include "gperson.h"
#include "cscene.h"


gPerson::gPerson(cScene * thisScene, dPerson * dp )
{

   xpos = dp->Xpos() * thisScene->ScaleFactor();
   ypos = (thisScene->TopPosition() - dp->BirthYear()) * thisScene->TimeScale();

   height = 40;

   float fmargin = 5;

   firstName = dp->firstName;
   lastName = dp->lastName;
   QString name = firstName + " "  + lastName;

   nameItem = thisScene->addSimpleText(name);
   float nameWidth = nameItem->boundingRect().right();
   nameItem->setPos(xpos - nameWidth/2,ypos);



   box = new QGraphicsRectItem(QRect(xpos - nameWidth/2 - fmargin , ypos, nameWidth + 2*fmargin, height));


   box->setBrush(QColor(00,19,20,25));
   box->setPen(QPen(QColor(Qt::black)));
   thisScene->addItem(box);



   qDebug() << "gPerson" << name << "x" << xpos <<"y" << ypos << "scale"<<thisScene->TimeScale() << "dataperson x" << dp->Xpos() << "Top position"<<thisScene->TopPosition();
}

void gPerson::Move(int x, int y){
   box->moveBy(x,y);
   nameItem->moveBy(x,y);
}
QPointF gPerson::TopHook(){
   float x1 = xpos + 0.5 * width;
   float y1 = ypos;
   return QPointF(x1,y1);
}
QPointF gPerson::BottomHook(){
   float x1 = xpos + 0.5 * width;
   float y1 = ypos + height;
   return QPointF(x1,y1);
}
