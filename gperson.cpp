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
  dp->setGraphicPerson(this);
  setFlags(QGraphicsItem::ItemIsFocusable | QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);

  width = 100; // this isn't right, it should be dynamic and based on relevant data; but that's not available till after Paint();
  height = 40;
  margin = 5;
  firstName = dp->FirstName();
  lastName = dp->LastName();
  key = dp->Key();
  myFont = new  QFont("Times", 12);

  personBoundingRect.setCoords(-1.0 * margin, 0, width, height);
  xpos = 0;  // it will be set the first time it is add to scene, using the toppoint on the scene.
}

gPerson::gPerson(QStringList  data){
    setFlags(QGraphicsItem::ItemIsFocusable | QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);

    key = "no_key";
    width = 100; // this isn't right, it should be dynamic and based on relevant data; but that's not available till after Paint();
    height = 40;
    myFont = new  QFont("Times", 12);
    xpos = 0;               // it will be set the first time it is add to scene, using the toppoint on the scene.
    birthYear = 0;
    deathYear = 0;
    margin = 5;
    personBoundingRect.setCoords(-1.0 * margin, 0, width, height);
    if (data.size() >= 8 && data[7].length() > 0){
        key = data[7];
    } else {
        key = data[2];
    }
    firstName = data[1];
    lastName = data[2];
    birthYear = data[3].toInt();
    if (birthYear == 0){
        qDebug() << lastName << "zero birth year a";
    }
    if (data[4].length() > 0){
        deathYear = data[4].toInt();
    } else {
        deathYear = 0;
        qDebug() << lastName << "Zero death year" ;
    }
    x_fromspreadsheet = data[5].toFloat();
    profession1 = data[6];

}

gPerson::~gPerson(){

}
/*void gPerson::LinkToDPersonBidirectional(dPerson* dperson) {
    dataPerson=dperson;
    dperson->setGraphicPerson(this);
    setKey(dperson->Key());
}
*/

void gPerson::AppendLink(cLink *link) {
    myLinks.append(link);
    //qDebug() << "gperson 49" << link->display();
    if (link->GPersonFrom() == this){
        if (link->GetPositionOnFromPerson() == "Top"){
            topLinks.append(link);
        }
    }
    else if (link->GPersonTo()==this){
        if (link->GetPositionOnToPerson() == "Bottom"){
            bottomLinks.append(link);
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
    foreach (cLink* link, * GetLinks()){
        if (link->GPersonFrom() == this){
           link->setPos(pos());
           link->update();
            qDebug() << "***" << link->GPersonFrom()->LastName() << link->GPersonTo()->LastName();
        }
    }
    QGraphicsItem::mouseReleaseEvent(event);
}



float gPerson::GetNameWidth(QPainter * painter) const {
    QString name = firstName + " "  + lastName;
    QFontMetrics fm=painter->fontMetrics();

    return fm.horizontalAdvance(name);
}
float gPerson::GetDatesWidth(QPainter * painter) const {
    QString years = QString::number(birthYear) + "--" + QString::number(deathYear);
    QFontMetrics fm=painter->fontMetrics();
    return fm.horizontalAdvance(years);
}
void gPerson::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){

    //dPerson * dp = getDPerson();
    double topMargin = 10.0;
    double lineHeight = 15.0;

    painter->setFont(*myFont);
    QColor Orange;
    Orange.setRgb(255,153,51);
    QBrush brush1;

    QPen pen(Qt::black,1);
    painter->setPen(pen);
    painter->setBrush(brush1);
    QString Profession =  Profession1();
    if (Profession ==  "linguist"){
        brush1.setColor(Orange);
    }
    if (Profession == "sociologist"){
            brush1.setColor(Qt::green);
    }
    if (Profession == "philosopher"){
           brush1.setColor(Qt::yellow);
           //qDebug() << "gperson 122 "<< brush1.color().name();
     }
    if (Profession == "psychologist"){
           brush1.setColor(Qt::cyan);
           //qDebug() << "gperson 122 "<< brush1.color().name();
     }
     //qDebug() << "gperson 122 "<< brush1.color().name();
    if (hasFocus()){
        painter->setPen(Qt::black);
        brush1.setColor(Qt::yellow);
    }
    QString name = firstName + " "  + lastName;
    QString years = QString::number(birthYear) + "--" + QString::number(deathYear);
    float namewidth = GetNameWidth(painter);
    float datewidth = GetDatesWidth(painter);

    if (datewidth > namewidth){
        width = datewidth;
    }else{
        width = namewidth;
    }

    QRectF personrect(0,0,width, height);
    painter->drawRect(personrect);
    //qDebug() << "gperson 122 "<< brush1.color().name();
    QRectF rect(0,0,namewidth,20);
    QRectF boundingRect1;
    painter->drawText(rect ,Qt::AlignHCenter,  name ,  & boundingRect1);

    QRectF boundingRect2 (boundingRect1);
    rect.moveTo(0,lineHeight);
    painter->drawText(rect, Qt::AlignHCenter,years, &boundingRect2);

    float margin = 5;
    personBoundingRect.setCoords(-1.0 * margin, 0, width, height);
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
            thislink->TopOffset(startingPoint + i * delta);
            i++;
            if (LastName() == "Sapir")
                qDebug() << "gperson SortLinks 2 " << thislink->GPersonFrom()->LastName();
        }

    }

    delta = (boxwidth * 0.5) / GetBottomLinks()->size();
    thisList = GetBottomLinks();
    LS = thisList->size();
    if (LS > 1){
        int i = 0;
        float startingPoint;
        startingPoint =  -1.0 * ( (LS/2.0  ) * delta ) ;
        qDebug() << "";
        foreach (cLink* thislink, * thisList){
            thislink->BottomOffset(startingPoint + i * delta);
            i++;
            if (LastName() == "Sapir")
               qDebug() << "gperson SortLinks 2 " << thislink->GPersonTo()->LastName();
        }

    }

}
void gPerson::write(QJsonObject & json) const {
    json["key"] = key;
    json["type"] = "person";
    json["firstName"] = firstName;
    json["lastName"] = lastName;
    json["xpos"] = x();
    json["ypos"] = y();
    json["height"] = height;

    json["birthYear"] = birthYear;
    json["deathYear"] = deathYear;




}
void gPerson::read(const QJsonObject &json){
    key = json["key"].toString();
    firstName = json["firstName"].toString();
    lastName = json["lastName"].toString();
    xpos = json["xpos"].toDouble();
    ypos = json["ypos"].toDouble();
    height = json["height"].toInt();
    birthYear = json["birthYear"].toInt();
    deathYear = json["deathYear"].toInt();
}
void gPerson::rememberPos(QPointF point){
    xpos = point.x();
    ypos = point.y();
}
