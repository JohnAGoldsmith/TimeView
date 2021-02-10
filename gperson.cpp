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
  //scaleFactor = thisScene->ScaleFactor();
  height = 40;
  margin = 5;
  firstName = dp->FirstName();
  lastName = dp->LastName();
}
gPerson::~gPerson(){

}
QRectF gPerson::boundingRect() const{
    //qDebug() << "bounding rect" << xpos <<ypos <<width << height;
    //return QRectF(xpos,ypos,width,height);
    return QRectF(0,0,width,100);
}
void gPerson::mousePressEvent(QGraphicsSceneMouseEvent * event){
    Pressed = true;
    QGraphicsItem::mousePressEvent(event);

    qDebug() << "gPerson clicked";
    // update();
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
    QRect rect(0,0,width,50);
    QRect boundingRect;
    painter->drawText(rect ,Qt::AlignHCenter,  name ,  & boundingRect);

    QString years = QString::number(dp->BirthYear()) + "--" + QString::number(dp->DeathYear());
    rect.setTop(lineHeight);
    painter->drawText(rect, Qt::AlignHCenter,years, &boundingRect);
    //qDebug() << "bounding rect" << boundingRect.top() << boundingRect.left() << boundingRect.bottom() << boundingRect.right() << "margin" << margin;
    //painter->drawRect(0,0,boundingRect.width() , 40);


    //QRectF boundingRectF = painter->boundingRect(rect,years);
    //float yearsWidth = yearsItem->boundingRect().right();
    //yearsItem->setPos(xpos-yearsWidth/2, ypos+15);
    //yearsItem->setAcceptedMouseButtons(0);





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
