#include <QGraphicsLineItem>
#include <QDebug>
#include <QStringList>
#include "clink.h"
#include "gperson.h"
#include "cscene.h"

cLink::cLink()
{
    QString fromPersonKey;
    QString toPersonKey;

    QString natureOfLink;
    gPerson* gPersonFrom;
    gPerson* gPersonTo;
}
cLink::cLink(QStringList & data){
    setAcceptedMouseButtons(0);
    fromPersonKey = data[1];
    toPersonKey = data[2];
    if (data.size() >= 11 && data[10].length() > 0){
        natureOfLink = data[10];
    }

}
cLink::~cLink(){

}

void cLink::attachGraphicalPersons(gPerson * person1, gPerson * person2){
 gPersonFrom = person1;
 gPersonTo = person2;
 person1->AppendLink(this);
 person2->AppendLink(this);
}

QRectF cLink::boundingRect() const {
    return QRectF();
}


void cLink::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    double fraction1(0.5);

    gPerson * gp1 = GPersonFrom();
    gPerson * gp2 = GPersonTo();
    QPointF point_start  (gp1->pos().x(), gp1->pos().y());
    QPointF point_end    (gp2->pos().x(), gp2->pos().y() + gp2->Height() );
    QPointF correction   (gp1->CenterX(),0);

    //qDebug() << 68 << gp1->CenterX();
    point_start += correction;
    point_end   += correction;

    //QGraphicsLineItem * line1 = new QGraphicsLineItem(QLineF(point_start,point_end));
    //addItem(line1);
    \
    double verticalDistance = point_end.y() - point_start.y();
    QPointF delta1 (0,fraction1 * verticalDistance);
    QPointF point1 = point_start + delta1;
    QPointF point2 (point_end.x(), point1.y());
    QGraphicsLineItem  *line2, *line3, *line4;

    if ( verticalDistance < 0.0){

        line2 = new QGraphicsLineItem(QLineF(point_start,point1));
        line3 = new QGraphicsLineItem(QLineF(point1, point2));
        line4 = new QGraphicsLineItem(QLineF(point2, point_end));

        //addItem(line2);
        //addItem(line3);
        //addItem(line4);
    }
}

