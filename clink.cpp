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
    bottomOffset = 0.0;
    topOffset = 0.0;
}
cLink::cLink(QStringList & data){
    gPersonFrom = NULL;
    gPersonTo = NULL;
    setAcceptedMouseButtons(0);
    fromPersonKey = data[1];
    toPersonKey = data[2];
    bottomOffset = 0.0;
    topOffset = 0.0;

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
    if (data[6] == "0"){ PositionOnToPerson =  "Right";}
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
  out += "Display a link: fromPersonKey ";
  out += fromPersonKey;
  out += " toPersonKey ";
  out += toPersonKey;
  out += " thru gPerson links";
  out += natureOfLink;
  return out;
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
    double fraction1(0.7);

    /* fix "centerX" function */
    float GPersonFromCenterX = 50;
    float GPersonToCenterX = 50;
    float willbewidth = 120;
    float willbeheight = 80;

    float y_distance1 = GPersonFrom()->scenePos().y() - GPersonTo()->scenePos().y();
    float y_distance2 = y_distance1 - GPersonTo()->Height();
    float x_distance = GPersonFrom()->scenePos().x() - GPersonTo()->scenePos().x() + (GPersonFromCenterX - GPersonToCenterX);
    QPointF start_point;
    QPointF end_point;
    QPen pen;

    if (natureOfLink == "teacher" || natureOfLink == "student"){
        pen.setColor(Qt::blue);
        pen.setWidth(3);
        painter->setPen(pen);
    }
    if (natureOfLink == "postDoc" || natureOfLink == "Semi-teacher" || natureOfLink == "influence"){
        pen.setColor(Qt::blue);
        pen.setWidth(3);
        pen.setStyle(Qt::DashLine);
        painter->setPen(pen);
     }
    if (natureOfLink == "colleagues") {
               pen.setColor(Qt::green);
               pen.setWidth(3);
               painter->setPen(pen);
    }
    if (natureOfLink == "hostile") {
               pen.setColor(Qt::red);
               pen.setWidth(3);
               painter->setPen(pen);
    }


    /* Now we do things in Link's coordinates, which is From */
    if (PositionOnFromPerson == "Top") {
       start_point  = QPointF(GPersonFromCenterX + topOffset,0 );
    }
    else if (PositionOnFromPerson == "Right") {
        start_point = QPoint(willbewidth, willbeheight * 0.5 );
    }
    else if (PositionOnFromPerson == "Left") {
        start_point = QPoint(0, willbeheight * 0.5 );
    }
    else{
        start_point  = QPointF(GPersonFromCenterX + topOffset,0 );
    }

    if (PositionOnToPerson == "Bottom") {
       end_point = QPointF(-1.0 * x_distance - bottomOffset + GPersonToCenterX, -1.0 * y_distance2);
    }
    else if (PositionOnToPerson == "Right") {
        end_point = QPoint(-1.0 * x_distance + willbewidth *0.5 ,  -1.0 * y_distance2 - 0.5 * willbeheight );
        //qDebug() << "####" << GPersonTo()->LastName();
    }
    else if (PositionOnToPerson == "Left") {
        //point_end = QPoint(-1.0 * willbeX, willbeY );
    }
    else {
        end_point = QPointF(-1.0 * x_distance , -1.0 * y_distance2);
    }



    QPointF point1 (start_point.x(), start_point.y() - fraction1 * y_distance2  );
    QPointF point2 (end_point.x(), point1.y());

    //painter->drawLine(start_point,end_point);

    painter->drawLine(start_point, point1);
    painter->drawLine(point1, point2);
    painter->drawLine(point2, end_point);

}

void cLink::write(QJsonObject & json) const{
 json["FromPersonKey"] = fromPersonKey;
 json["ToPersonKey"] = toPersonKey;
 json["NatureOfLink"] = natureOfLink;
 json["PositionOnFromPerson"] = PositionOnFromPerson;
 json["PositionOnToPerson"] = PositionOnToPerson;
}
void cLink::read(QJsonObject & json)  {
 fromPersonKey = json["FromPersonKey"].toString();
 toPersonKey = json["ToPersonKey"].toString() ;
 natureOfLink = json["NatureOfLink"].toString();

}
