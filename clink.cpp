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
    update();

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

    float  willbe_tophook = 50.0;
    float willbe_bottomhook = 50.0;
    float y_distance = GPersonTo()->scenePos().y() - GPersonFrom()->scenePos().y() + GPersonTo()->Height();
    float x_distance = GPersonTo()->scenePos().x() - GPersonFrom()->scenePos().x();
    QPointF point_start (willbe_tophook,0 );
    QPointF point_end   (x_distance + willbe_bottomhook ,y_distance);

    return QRectF( point_start, point_end );
}


void cLink::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    double fraction1(0.9);



    float  willbe_tophook = 50.0;
    float willbe_bottomhook = 50.0;
    float y_distance = GPersonTo()->scenePos().y() - GPersonFrom()->scenePos().y() + GPersonTo()->Height();
    float x_distance = GPersonTo()->scenePos().x() - GPersonFrom()->scenePos().x();

    QPointF point_start (willbe_tophook,0 );
    QPointF point_end   (x_distance + willbe_bottomhook ,y_distance);
    //painter->drawLine(point_start,point_end);

    //qDebug() << 50 <<GPersonFrom()->LastName() << GPersonFrom()->scenePos().x() << GPersonFrom()->scenePos().y();
    //qDebug() << 51 <<GPersonTo()->LastName() << GPersonTo()->scenePos().x() << GPersonTo()->scenePos().y();
    qDebug() << 52 << GPersonFrom()->LastName() << GPersonTo()->LastName() <<  x_distance << y_distance ;

    float verticalDistance = y_distance;
    //qDebug() << 59 << gp1->LastName() << gp2->LastName();
    //qDebug() << 59 << "vertical distance"<< verticalDistance;

    QPointF point1 (point_start.x(),  fraction1 * verticalDistance  );
    QPointF point2 (point_end.x(), point1.y());


    if ( verticalDistance < 0.0){

        painter->drawLine(point_start,point1);
        painter->drawLine(point1, point2);
        painter->drawLine(point2,point_end);

        //painter->drawLine(point_start, point1);

        //line2 = new QGraphicsLineItem(QLineF(point_start,point1));
        //line3 = new QGraphicsLineItem(QLineF(point1, point2));
        //line4 = new QGraphicsLineItem(QLineF(point2, point_end));

        //update();

        //addItem(line2);
        //addItem(line3);
        //addItem(line4);
    }
}

