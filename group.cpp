#include "group.h"
#include <QDebug>
#include <QPainter>
cGroup::cGroup(QStringList data)
{
   setZValue(-1);
   key = data[1];
   name = data[2];
   scene_x = data[3].toFloat();
   scene_y = data[4].toFloat();
   height = data[5].toFloat();
   width = data[6].toFloat();

   myBoundingRect = QRectF(0,0,height,width);
   setFlags(QGraphicsItem::ItemIsFocusable | QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
   setAcceptedMouseButtons(Qt::LeftButton);
}
cGroup::cGroup (QString k, QString n, float x, float y, float h, float w ){
    setZValue(-1);
    key = k;
    name = n;
    scene_x = x;
    scene_y = y;
    height = h;
    width = w;
    myBoundingRect = QRectF(0,0,height,width);
    setFlags(QGraphicsItem::ItemIsFocusable | QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
    setAcceptedMouseButtons(Qt::LeftButton);
    qDebug() << "created new group" << name;
}


cGroup::~cGroup(){



}

QString cGroup::export2csv(){
   QStringList  temp;
   temp.append("G");
   temp.append(key );
   temp.append(name);
   temp.append(QString::number(pos().x()) );
   temp.append(QString::number(pos().y()) );
   temp.append(QString::number(height));
   temp.append(QString::number(width));
   return temp.join(",");
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
    painter->drawRoundedRect(0,0,width,height,100,100);
    //painter->drawEllipse(0,0,height,width);
}
/*
void cGroup::mousePressEvent(QGraphicsSceneMouseEvent * event){
    QGraphicsItem::mousePressEvent(event);
    qDebug() << "Group has focus." << key;

}
void cGroup::mouseMoveEvent(QGraphicsSceneMouseEvent * event)  {
    QGraphicsItem::mouseMoveEvent(event);
    qDebug() << "group noticed mouse movement.";
}
*/

void cGroup::mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event){

}
