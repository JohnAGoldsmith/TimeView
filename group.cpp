#include "group.h"
#include <QDebug>
#include <QPainter>
cGroup::cGroup(QStringList data)
{
   setZValue(-1);
   scene_x = data[3].toFloat();
   scene_y = data[4].toFloat();
   height = 400;
   width = 600;
   name = data[5];
   key = name;

   myBoundingRect = QRectF(0,0,height,width);

   setFlags(QGraphicsItem::ItemIsFocusable | QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
   setAcceptedMouseButtons(Qt::LeftButton);

}
cGroup::~cGroup(){

}

QRectF cGroup::boundingRect() const
{
  return  myBoundingRect;
}

void cGroup::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){

    QPen pen;
    pen.setColor(Qt::blue);
    pen.setWidth(5);
    painter->setPen(pen);
    QBrush brush;
    brush.setColor(Qt::yellow);
    brush.setStyle(Qt::SolidPattern);
    painter->setBrush(brush);
    //painter->drawRect(0,0,width,height,5,5);
    painter->drawEllipse(0,0,100,200);
}

