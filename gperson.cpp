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
   QString name = dp->firstName + " "  + dp->lastName;
   nameItem = thisScene->addSimpleText(name);
   float nameWidth = nameItem->boundingRect().right();
   nameItem->setPos(xpos - nameWidth/2,ypos);

   box = new QGraphicsRectItem(QRect(xpos - nameWidth/2 - fmargin , ypos, nameWidth + 2*fmargin, height));
   box->setBrush(QColor(00,19,20,25));
   box->setPen(QPen(QColor(Qt::black)));
   thisScene->addItem(box);

   QString years = QString::number(dp->BirthYear()) + "--" + QString::number(dp->DeathYear());
   yearsItem = thisScene->addSimpleText(years);
   float yearsWidth = yearsItem->boundingRect().right();
   yearsItem->setPos(xpos-yearsWidth/2, ypos+15);

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
