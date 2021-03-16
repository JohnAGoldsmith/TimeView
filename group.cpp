#include "group.h"
#include <QDebug>
cGroup::cGroup(QStringList data)
{
   x = 0;
   y = data[3].toFloat();
   height = 50;
   width = 50;
   name = data[5];
   qDebug() << x << y << width << height;
   QGraphicsRectItem * rect = new QGraphicsRectItem(x,y,width,height,this);
}
cGroup::~cGroup(){

}

QRectF cGroup::boundingRect() const
{

  return QGraphicsRectItem::boundingRect();
}
void cGroup::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
     QGraphicsRectItem::paint(painter, option, widget);
}
