#include <QGraphicsItem>
#include <QGraphicsTextItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStylePainter>
#include <QGradient>
#include <QDebug>
#include <QColor>
#include <QFontMetrics>
#include "gperson.h"
#include "cscene.h"

class cLink;

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

  float totalwidth(0.0);
  float namewidth = GetNameWidth();
  float datewidth = GetDatesWidth();
  if (datewidth > namewidth)
      totalwidth = datewidth;
  else
      totalwidth = namewidth;
  personBoundingRect.setCoords(-1.0 * margin, 0, totalwidth, height);

}
gPerson::~gPerson(){

}

void gPerson::AppendLink(cLink *link) {myLinks.append(link);}

QRectF gPerson::boundingRect() const  {
    return personBoundingRect;
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
float gPerson::CenterX(){
   return GetNameWidth() * 0.5;
}
float gPerson::GetNameWidth() const {
    QString name = firstName + " "  + lastName;
    QPainter painter;
    painter.setFont(QFont("Times", 10));
    QFontMetrics fm=painter.fontMetrics();
    return fm.horizontalAdvance(name);
}
float gPerson::GetDatesWidth() const {
    QString years = QString::number(getDPerson()->BirthYear()) + "--" + QString::number(getDPerson()->DeathYear());
    QPainter painter;
    painter.setFont(QFont("Times", 10));
    QFontMetrics fm=painter.fontMetrics();
    return fm.horizontalAdvance(years);
}
void gPerson::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){

    dPerson * dp = getDPerson();
    double topMargin = 10.0;

    double lineHeight = 15.0;
    double totalWidth = 0;
    QBrush brush (Qt::Dense6Pattern);
    if (hasFocus()){
        painter->setPen(Qt::blue);
        brush.setColor(Qt::blue);
    }else{
          brush.setColor(Qt::red);
    }


    QString name = firstName + " "  + lastName;
    painter->setFont(QFont("Times", 10));

    float namewidth = GetNameWidth();

    QRectF rect(0,0,namewidth,20);
    QRectF boundingRect1;
    painter->drawText(rect ,Qt::AlignHCenter,  name ,  & boundingRect1);

    QRectF boundingRect2 (boundingRect1);
    QString years = QString::number(dp->BirthYear()) + "--" + QString::number(dp->DeathYear());
    rect.moveTo(0,lineHeight);
    painter->drawText(rect, Qt::AlignHCenter,years, &boundingRect2);


    float totalwidth(0.0);
    float datewidth = GetDatesWidth();

    if (datewidth > namewidth){
        totalwidth = datewidth;
    }else{
        totalwidth = namewidth;
    }

    QRectF completeRect;
    completeRect.setCoords(-1 * margin, 0,  totalwidth, height);
    painter->drawRect(completeRect);
    painter->fillRect(completeRect,brush);


    //QGraphicsItem::paint(painter, option, widget);

}


void gPerson::Move(int x, int y){
   box->moveBy(x,y);
   nameItem->moveBy(x,y);
}
