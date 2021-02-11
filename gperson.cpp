#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStylePainter>
#include <QGradient>
#include <QDebug>
#include <QColor>
#include "gperson.h"
#include "cscene.h"

gPerson::gPerson(){

}
gPerson::gPerson( dPerson * dp )
{
   dataPerson = dp;
  setFlags(QGraphicsItem::ItemIsFocusable | QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);

  height = 40;
  margin = 5;
  firstName = dp->FirstName();
  lastName = dp->LastName();
}
gPerson::~gPerson(){

}
QRectF gPerson::boundingRect() const{
    return boundingBox;
}
void gPerson::mousePressEvent(QGraphicsSceneMouseEvent * event){
    qDebug() << "gPerson clicked" << "x" << event->scenePos().x() << "y" << event->scenePos().y();
    QGraphicsItem::mousePressEvent(event);
    update();
}
void gPerson::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
    Pressed = false;
    update();
    QGraphicsItem::mouseReleaseEvent(event);
}
void gPerson::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    dPerson * dp = getDPerson();
    double topMargin = 10.0;
    //double sideMargin = 10.0;
    double lineHeight = 15.0;
    width = 140.0;
    QString name = firstName + " "  + lastName;
    painter->setFont(QFont("Times", 10));
    if (hasFocus()){
        painter->setPen(Qt::blue);
    }

    QRectF rect(0,0,width,50);
    QRectF boundingRect1;
    painter->drawText(rect ,Qt::AlignHCenter,  name ,  & boundingRect1);
    painter->drawRect( boundingRect1);
    boundingBox = boundingRect1;
    centerX = boundingRect1.width() * 0.5;
    //qDebug() << "paint g person midpoint" << centerX;


    QRectF boundingRect2 (boundingRect1);
    QString years = QString::number(dp->BirthYear()) + "--" + QString::number(dp->DeathYear());
    rect.setTop(lineHeight);
    painter->drawText(rect, Qt::AlignHCenter,years, &boundingRect2);
    painter->drawRect(boundingRect2);

    boundingBox = boundingBox.united(boundingRect2);
    painter->drawRect(boundingBox);


}


void gPerson::Move(int x, int y){
   box->moveBy(x,y);
   nameItem->moveBy(x,y);
}
QPointF gPerson::TopHook(){
   float x1 = xpos;
   float y1 = ypos;
   return QPointF(x1,y1);
}
QPointF gPerson::BottomHook(){
   float x1 = xpos ;
   float y1 = ypos + height;
   return QPointF(x1,y1);
}
