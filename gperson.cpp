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
#include "clink.h"



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
  myFont = new  QFont("Times", 12);
  QPainter painter;
  painter.setFont(* myFont);

  float totalwidth(0.0);
  float namewidth = GetNameWidth(& painter);
  float datewidth = GetDatesWidth(& painter );
  if (datewidth > namewidth)
      totalwidth = datewidth;
  else
      totalwidth = namewidth;
  qDebug()<< "39 constructor"<<totalwidth;
  topHook.setX(totalwidth * 0.5);
  topHook.setY(0);
  bottomHook.setX(totalwidth * 0.5);
  bottomHook.setY(height);
  personBoundingRect.setCoords(-1.0 * margin, 0, totalwidth, height);

}
gPerson::~gPerson(){

}


void gPerson::AppendLink(cLink *link) {
    myLinks.append(link);
    if (link->GPersonFrom() == this){
        if (link->GetPositionOnFromPerson() == Top){
            topLinks.append(link);
        } else{
            qDebug() << 59 << "We have a From person not on Top";
        }

    }
    SortLinks();
}

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



float gPerson::CenterX(QPainter * painter  ){
   return GetNameWidth(painter ) * 0.5;
}

float gPerson::GetNameWidth(QPainter * painter) const {
    QString name = firstName + " "  + lastName;
    QFontMetrics fm=painter->fontMetrics();
    return fm.horizontalAdvance(name);
}
float gPerson::GetDatesWidth(QPainter * painter) const {
    QString years = QString::number(getDPerson()->BirthYear()) + "--" + QString::number(getDPerson()->DeathYear());
    QFontMetrics fm=painter->fontMetrics();
    return fm.horizontalAdvance(years);
}
void gPerson::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){

    dPerson * dp = getDPerson();
    double topMargin = 10.0;

    double lineHeight = 15.0;
    double totalWidth = 0;
    painter->setFont(*myFont);
    QColor Orange;
    Orange.setRgb(255,153,51);
    QBrush brush (Qt::SolidPattern);
    QPen pen(Qt::black,10);
    painter->setPen(pen);
    if (hasFocus()){
        painter->setPen(Qt::white);
        brush.setColor(Qt::blue);
    }else{
          brush.setColor(Orange );
          painter->setPen(Qt::black);
    }


    QString name = firstName + " "  + lastName;
    QString years = QString::number(dp->BirthYear()) + "--" + QString::number(dp->DeathYear());
    float namewidth = GetNameWidth(painter);
    float datewidth = GetDatesWidth(painter);
    float totalwidth(0.0);
    if (datewidth > namewidth){
        totalwidth = datewidth;
    }else{
        totalwidth = namewidth;
    }
    QRectF completeRect;
    completeRect.setCoords(-1 * margin, 0,  totalwidth, height);
    painter->fillRect(completeRect,brush);

    QRectF rect(0,0,namewidth,20);
    QRectF boundingRect1;
    painter->drawText(rect ,Qt::AlignHCenter,  name ,  & boundingRect1);

    QRectF boundingRect2 (boundingRect1);
    rect.moveTo(0,lineHeight);
    painter->drawText(rect, Qt::AlignHCenter,years, &boundingRect2);
    //QGraphicsItem::paint(painter, option, widget);

}
void gPerson::SortLinks(){
    float boxwidth = 100.0; // should be GetNameWidth();
    float delta = (boxwidth * 0.5) / GetTopLinks()->size();

    QList<cLink*> * thisList = GetTopLinks();
    int LS = thisList->size();
    if (LS > 1){
        int i = 0;
        float startingPoint;
        startingPoint =  -1.0 * ( (LS/2.0  ) * delta ) ;
        foreach (cLink* thislink, * thisList){
            thislink->Offset(startingPoint + i * delta);
            i++;
        }
    }
}

