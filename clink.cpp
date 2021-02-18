#include <QGraphicsLineItem>
#include <QDebug>
#include <QStringList>
#include <QColor>
#include <QPen>
#include "clink.h"
#include "gperson.h"
#include "cscene.h"

cLink::cLink()
{
    QString fromPersonKey ("");
    QString toPersonKey("");

    QString natureOfLink;
    gPerson* gPersonFrom;
    gPerson* gPersonTo;
    offset = 0.0;
}
cLink::cLink(QStringList & data){
    gPersonFrom = NULL;
    gPersonTo = NULL;
    setAcceptedMouseButtons(0);
    fromPersonKey = data[1];
    toPersonKey = data[2];

    if (data.size() >= 11 && data[10].length() > 0){
        natureOfLink = data[10];
    }
    if (data[3] == "0"){ PositionOnFromPerson == "Right";}
    else if (data[3] == "90"){ PositionOnFromPerson="Top";}
    else if (data[3] == "180"){PositionOnFromPerson = "Left";}
    else if (data[3] == "-180"){PositionOnFromPerson = "Left";}
    else if (data[3] == "270") {PositionOnFromPerson = "Bottom";}
    else if (data[3] == "-90") {PositionOnFromPerson = "Bottom";}
    else {PositionOnFromPerson == "Not defined";}
    if (data[6] == 0){ PositionOnToPerson ==  "Right";}
    else if (data[6] == "90"){ PositionOnToPerson="Top";}
    else if (data[6] == "180"){PositionOnToPerson = "Left";}
    else if (data[6] == "-180"){PositionOnToPerson = "Left";}
    else if (data[6] == "270") {PositionOnToPerson = "Bottom";}
    else if (data[6] == "-90") {PositionOnToPerson = "Bottom";}
    else {PositionOnToPerson = "Not defined";}

    update();
}
cLink::~cLink(){

}
QString cLink::display()const {
  QString out;
  out += " fromPersonKey";
  out += fromPersonKey;
  out += " toPersonKey ";
  out += toPersonKey;
  out += " thru gPerson links";
  if (gPersonFrom){
      out += gPersonFrom->LastName();
    } else{
      out += "No From link";
  }
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
    QPointF point_end;
    QPointF point_start;
    QPen pen;
    if (natureOfLink == "teacher"){
        pen.setColor(Qt::blue);
        pen.setWidth(3);
        painter->setPen(pen);
    } else {
        if (natureOfLink == "postDoc"){
        pen.setColor(Qt::blue);
        pen.setWidth(3);
        pen.setStyle(Qt::DashLine);
        painter->setPen(pen);
        } else {if (natureOfLink == "colleagues") {
               pen.setColor(Qt::green);
               pen.setWidth(3);
               painter->setPen(pen);
                }
            }
    }

    point_start  = QPointF(willbe_tophook + offset,0 );
    qDebug() << 61 << "offset:"<< offset;
    point_end = QPoint(x_distance + willbe_bottomhook ,y_distance);
    float verticalDistance = y_distance;

    QPointF point1 (point_start.x(),  fraction1 * verticalDistance  );
    QPointF point2 (point_end.x(), point1.y());

    if ( verticalDistance < 0.0){

        painter->drawLine(point_start,point1);
        painter->drawLine(point1, point2);
        painter->drawLine(point2,point_end);

    }
}

void cLink::write(QJsonObject & json) const{
 json["FromPersonKey"] = fromPersonKey;
 json["ToPersonKey"] = toPersonKey;
 json["NatureOfLink"] = natureOfLink;
}
