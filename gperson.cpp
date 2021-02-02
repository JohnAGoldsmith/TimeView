#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QGraphicsScene>
#include <QStylePainter>
#include <QGradient>
#include <QDebug>
#include "gperson.h"
#include "cscene.h"


gPerson::gPerson(cScene * thisScene, dPerson * dp )
{
   // qDebug() << "gPerson constructor"<<  dp-> lastName;
   xpos = dp->Xpos() * thisScene->ScaleFactor();
   ypos = (thisScene->TopPosition() - dp->BirthYear()) * thisScene->TimeScale();
   //qDebug() << dp->Xpos() << thisScene->TimeScale();
   //qDebug() << topPosition << dp->BirthYear() << thisScene->TimeScale() << xpos << ypos;
   width = 100;
   height = 40;
   firstName = dp->firstName;
   lastName = dp->lastName;
   QString name = firstName + " "  + lastName;
   box = new QGraphicsRectItem(QRect(xpos, ypos, width, height));
   box->setBrush(QColor(00,190,200,255));
   thisScene->addItem(box);
   nameItem = thisScene->addSimpleText(name);
   nameItem->setPos(xpos,ypos);

}

void gPerson::Move(int x, int y){
   box->moveBy(x,y);
   nameItem->moveBy(x,y);
}
