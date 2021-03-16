#include <QGraphicsLineItem>
#include <QDebug>
#include <QStringList>
#include <QColor>
#include <QPen>
#include "clink.h"
#include "gperson.h"
#include "cscene.h"

/* Used when reading json file */
cLink::cLink()
{
    fromPersonKey = "";
    toPersonKey="";
    natureOfLink= "";
    gPersonFrom  =  NULL;
    gPersonTo = NULL;
    bottomOffset = 0.0;
    topOffset = 0.0;
    visible = true;
    chosen = false;
    proportion1 = 0.6;
    startPoint = QPoint(0.0, 0.0);
    endPoint = QPoint(0.0, 0.0);
    update();
}

/*  Used when reading csv file */
cLink::cLink(QStringList & data){
    gPersonFrom = NULL;
    gPersonTo = NULL;
    setAcceptedMouseButtons(0);
    fromPersonKey = data[1];
    toPersonKey = data[2];
    bottomOffset = 0.0;
    topOffset = 0.0;
    visible = true;
    chosen = false;
    proportion1 = 0.6;
    startPoint =QPoint(0.0, 0.0);
    endPoint = QPoint(0.0, 0.0);

    if (data.size() >= 11 && data[10].length() > 0){
        natureOfLink = data[10];
    }
    if (data[3] == "0"){ PositionOnFromPerson = "Right";}
    else if (data[3] == "90"){ PositionOnFromPerson="Top";}
    else if (data[3] == "180"){PositionOnFromPerson = "Left";}
    else if (data[3] == "-180"){PositionOnFromPerson = "Left";}
    else if (data[3] == "270") {PositionOnFromPerson = "Bottom";}
    else if (data[3] == "-90") {PositionOnFromPerson = "Bottom";}
    else {PositionOnFromPerson = "Not defined";}
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
void cLink::setPersonFrom(gPerson * person){
    gPersonFrom = person;
}
void cLink::setPersonTo(gPerson * person){
    gPersonTo = person;
}
QString cLink::getKey() const {
    return fromPersonKey + ":" + toPersonKey;
}

void cLink::BottomOffset(float f){
    bottomOffset = f;
}
void cLink::TopOffset(float f){
    topOffset = f;
}
QString cLink::export2csv(){
    QStringList temp;
    temp << "L" << fromPersonKey << toPersonKey <<
            PositionOnFromPerson << PositionOnToPerson <<
            QString::number(bottomOffset) <<
            QString::number(topOffset) <<
            QString::number(proportion1) <<
            QString::number(startPoint.x()) <<
            QString::number(startPoint.y()) <<
            QString::number(endPoint.x()) <<
            QString::number(endPoint.y()) <<
            natureOfLink;
   return temp.join(",");
}
void cLink::importFromCSV(QString line){
  QStringList temp (line.split(","));
  fromPersonKey = temp[0];
  toPersonKey = temp[1];
  PositionOnFromPerson = temp[2];
  PositionOnToPerson = temp[3];
  bottomOffset = temp[4].toFloat();
  topOffset = temp[4].toFloat();
  proportion1 = temp[5].toFloat();
  startPoint.setX( temp[6].toFloat() );
  startPoint.setY( temp[7].toFloat() );
  endPoint.setX( temp[6].toFloat() );
  endPoint.setY( temp[7].toFloat() );
}
void cLink::importFromCSVlegacy(QString line){

}
QString cLink::display()const {
  QString out;
  out += "Display a link: fromPersonKey ";
  out += fromPersonKey;
  out += " toPersonKey ";
  out += toPersonKey;
  out += " thru gPerson links ";
  out += natureOfLink;
  out += "Position on From Person: ";
  out += PositionOnFromPerson;
  out += "Position on To Person: ";
  out += PositionOnToPerson;
  out += "Visible? ";
  out += visible;
  return out;
}
void cLink::attachPersons(gPerson * person1, gPerson * person2){
 gPersonFrom = person1;
 gPersonTo = person2;
 person1->AppendLink(this);
 person2->AppendLink(this);
}

QRectF cLink::boundingRect() const {
  return QRectF(startPoint, endPoint);
}


void cLink::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {

    if (! GPersonFrom())
       qDebug() << "link painting but no link from.";
    if (! GPersonTo())
       qDebug() << "link painting but no link to.";


    /* fix "centerX" function */
    float GPersonFromCenterX = 50;
    float GPersonToCenterX = 50;
    float willbewidth = 120;
    float willbeheight = 80;

    float y_distance1 = GPersonFrom()->scenePos().y() - GPersonTo()->scenePos().y();
    float y_distance2 = y_distance1 - GPersonTo()->Height();
    float x_distance = GPersonFrom()->scenePos().x() - GPersonTo()->scenePos().x() + (GPersonFromCenterX - GPersonToCenterX);
    QPen pen;
    QPointF point1, point2;

    if (false)
        qDebug() << 108 << "just painting" << fromPersonKey << toPersonKey << chosen;
    if (! GPersonFrom()->Visible()){
        pen.setStyle(Qt::DotLine);
        painter->setPen(pen);
        qDebug() << "clink 111" << "invisible link...";
    }
    else if (chosen){
        qDebug() << "clink selected line 115"<< display();
        pen.setColor(Qt::red);
        pen.setWidth(6);
        painter->setPen(pen);
    }
    else if (natureOfLink == "teacher" || natureOfLink == "student"){
        pen.setColor(Qt::blue);
        pen.setWidth(1);
        painter->setPen(pen);
    }
    else if (natureOfLink == "postDoc" || natureOfLink == "Semi-teacher" || natureOfLink == "influence"){
        pen.setColor(Qt::blue);
        pen.setWidth(3);
        pen.setStyle(Qt::DashLine);
        painter->setPen(pen);
     }
    else if (natureOfLink == "colleagues") {
               pen.setColor(Qt::green);
               pen.setWidth(3);
               painter->setPen(pen);
    }
    else {
               pen.setColor(Qt::black);
               pen.setWidth(3);
               painter->setPen(pen);
    }


    /* Now we do things in Link's coordinates, which is From */

    /*   startpoint   */
    if (PositionOnFromPerson == "Top") {
        startPoint = QPointF(topOffset + GPersonFromCenterX,0);
    }
    else if (PositionOnFromPerson == "Right") {
        startPoint = QPoint(GPersonFrom()->Width(), GPersonFrom()->Height() * 0.5 );
    }
    else if (PositionOnFromPerson == "Left") {
        startPoint = QPoint(0, willbeheight * 0.5 );
    }
    else{
        startPoint  = QPointF(GPersonFromCenterX + topOffset,0 );
    }

    /*  endpoint */
    if (PositionOnToPerson == "Bottom") {
       endPoint = QPointF(-1.0 * x_distance - bottomOffset + GPersonToCenterX, -1.0 * y_distance2);
    }
    else if (PositionOnToPerson == "Right") {
        endPoint = QPoint(-1.0 * x_distance + GPersonTo()->Width() ,  -1.0 * y_distance2  - 0.5 *  GPersonTo()->Height()  );
    }
    else if (PositionOnToPerson == "Left") {
         endPoint = QPoint(-1.0 * x_distance ,  -1.0 * y_distance2 - 0.5 * GPersonTo()->Height() );
    }
    else {
        endPoint = QPointF(-1.0 * x_distance , -1.0 * y_distance2);
    }
    /*   point1 and point2    */

    if (PositionOnFromPerson=="Top"){
            if (PositionOnToPerson == "Right"){
                point1.setX(startPoint.x());
                point1.setY( startPoint.y() - proportion1 * y_distance2  );
                point2.setX (endPoint.x());
                point2.setY( point1.y() );
            }
    }
    if (PositionOnFromPerson=="Top"){
            if (PositionOnToPerson == "Bottom"){
                point1.setX(startPoint.x());
                point1.setY( startPoint.y() - proportion1 * y_distance2  );
                point2.setX (endPoint.x());
                point2.setY( point1.y() );
            }
    }
    if (PositionOnFromPerson=="Top"){
            if (PositionOnToPerson == "Left"){
                point1.setX(startPoint.x());
                point1.setY( endPoint.y() ); //- fraction1 * y_distance2  );
                point2.setX (point1.x());
                point2.setY( point1.y() );
            }
    }

    if (PositionOnFromPerson=="Right"){
        if (PositionOnToPerson == "Bottom"){
            point1.setX(endPoint.x() );
            point1.setY( startPoint.y()  );
            point2.setX (point1.x());
            point2.setY( point1.y() );
        }
    }
    if (PositionOnFromPerson=="Right"){
          point1.setY( startPoint.y()   );
          if (PositionOnToPerson == "Left"){
              float local_x_distance = startPoint.x() - endPoint.x();
              point1.setX(startPoint.x() - proportion1 * local_x_distance );
              point2.setX (point1.x());
              point2.setY( endPoint.y() );
          }
      }


    if (PositionOnFromPerson=="Left"){
        if (PositionOnToPerson == "Right"){
            float local_x_distance = startPoint.x() - endPoint.x();
            point1.setX(startPoint.x() - proportion1 * local_x_distance);
            point1.setY( startPoint.y()   );
            point2.setX (point1.x());
            point2.setY( endPoint.y() );
        }
    }
    if (PositionOnFromPerson=="Left"){
        if (PositionOnToPerson == "Bottom"){
            float local_x_distance = startPoint.x() - endPoint.x();
            point1.setX(startPoint.x() - proportion1 * local_x_distance);
            point1.setY( startPoint.y()   );
            point2.setX (endPoint.x());
            point2.setY( point1.y() );
        }
    }
    if (PositionOnFromPerson=="Left"){
        if (PositionOnToPerson == "Top"){
            float local_x_distance = startPoint.x() - endPoint.x();
            point1.setX(startPoint.x() - proportion1 * local_x_distance);
            point1.setY( startPoint.y()   );
            point2.setX (endPoint.x());
            point2.setY( point1.y() );
        }
    }

    //painter->drawLine(start_point,end_point);




    painter->drawLine(startPoint, point1);
    painter->drawLine(point1, point2);
    painter->drawLine(point2, endPoint);

}

void cLink::write(QJsonObject & json) const{
 json["Key"] = getKey();
 json["FromPersonKey"] = fromPersonKey;
 json["ToPersonKey"] = toPersonKey;
 json["NatureOfLink"] = natureOfLink;
 json["PositionOnFromPerson"] = PositionOnFromPerson;
 json["PositionOnToPerson"] = PositionOnToPerson;
 json["Visible"] = visible;
 json["bottomOffset"] = bottomOffset;
 json["topOffset"] = topOffset;
 json["proportion1"] = proportion1;
}

void cLink::read(QJsonObject & json)  {
 fromPersonKey = json["FromPersonKey"].toString();
 toPersonKey = json["ToPersonKey"].toString() ;
 natureOfLink = json["NatureOfLink"].toString();
 PositionOnFromPerson = json["PositionOnFromPerson"].toString();
 PositionOnToPerson = json["PositionOnToPerson"].toString();
 visible = json["Visible"].toBool();
 BottomOffset ( json["bottomOffset"].toDouble() );
 TopOffset ( json["topOffset"].toDouble() );
 proportion1 = json["proportion1"].toDouble();
}
