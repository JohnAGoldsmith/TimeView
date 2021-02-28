#include <QGraphicsItem>
#include <QGraphicsTextItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStylePainter>
#include <QGradient>
#include <QDebug>
#include <QColor>
#include <QPixmap>
#include <QImage>
#include <QFontMetrics>
#include "gperson.h"
#include "cscene.h"
#include "clink.h"



gPerson::gPerson(){
 setFlags(QGraphicsItem::ItemIsFocusable | QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
 myFont = new  QFont("Times", 12);

 width = 100; // this isn't right, it should be dynamic and based on relevant data; but that's not available till after Paint();
 height = 40;
 margin = 5;
 visible=true;
 personBoundingRect.setCoords(-1.0 * margin, 0, width, height);
 xpos = 0;  // it will be set the first time it is add to scene, using the toppoint on the scene.
 selectedLinkSet = "top";
 selectedLink = NULL;
}



gPerson::gPerson( dPerson * dp ) // this is not currently used at all.
{
  setFlags(QGraphicsItem::ItemIsFocusable | QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);

  width = 100; // this isn't right, it should be dynamic and based on relevant data; but that's not available till after Paint();
  height = 40;
  margin = 5;
  firstName = dp->FirstName();
  lastName = dp->LastName();
  key = dp->Key();
  myFont = new  QFont("Times", 12);
   visible=true;
  personBoundingRect.setCoords(-1.0 * margin, 0, width, height);
  xpos = 0;  // it will be set the first time it is add to scene, using the toppoint on the scene.
   selectedLinkSet = "top";
    selectedLink = NULL;
}





gPerson::gPerson(QStringList  data){ // this is used when starting from data in a spreadsheet;
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
    visible=true;
     selectedLinkSet = "top";
      selectedLink = NULL;
}

gPerson::~gPerson(){

}


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


void gPerson::focusInEvent(QFocusEvent * event){
    ShowSelectedLinkSet();
    QGraphicsItem::focusInEvent(event);
}
void gPerson::focusOutEvent(QFocusEvent * event){
    qDebug() << "gperson line 124 "<< LastName();
    UnselectAllLinks();
    QGraphicsItem::focusOutEvent(event);
}

void gPerson::mousePressEvent(QGraphicsSceneMouseEvent * event){
    qDebug() << "gPerson clicked" << "x" << event->scenePos().x() << "y" << event->scenePos().y();

    QGraphicsItem::mousePressEvent(event);
    update();
}

void gPerson::mouseMoveEvent(QGraphicsSceneMouseEvent * event){
    foreach (cLink* link, * GetLinks()){
        if (link->GPersonFrom() == this){
           link->setPos(scenePos());
           link->update();
            //qDebug() << "***" << link->GPersonFrom()->LastName() << link->GPersonTo()->LastName();
        }
    }
    QGraphicsItem::mouseMoveEvent(event);
}



void gPerson::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
    if (event->modifiers()==Qt::ShiftModifier){
        if (visible){
          visible = false;
          update();
          foreach(cLink* link, myLinks){
             link->Invisible();
             link->update();
          }
        } else {
            visible = true;
            update();
            foreach(cLink * link, myLinks){
                link->Visible();
                link->update();
            }
        }
    }
   QGraphicsItem::mouseReleaseEvent(event);
}

void gPerson::mouseDoubleClickEvent (QGraphicsSceneMouseEvent * event) {
    //QueryKeyboardModifiers
    QGraphicsItem::mouseDoubleClickEvent(event);
}



float gPerson::GetTextWidth(QPainter * painter, QString string) const{
    QFontMetrics fm=painter->fontMetrics();
    return fm.horizontalAdvance(string);
}
float gPerson::GetTextHeight(QPainter * painter) const{
    QFontMetrics fm=painter->fontMetrics();
    return fm.height( );
}
float gPerson::GetNameWidth(QPainter * painter) const {
    QString name = firstName + " "  + lastName;
    QFontMetrics fm=painter->fontMetrics();
    return fm.horizontalAdvance(name);
}
float gPerson::GetNameHeight(QPainter * painter) const {
    QString name = firstName + " "  + lastName;
    QFontMetrics fm=painter->fontMetrics();
    return fm.height();
}

float gPerson::GetDatesWidth(QPainter * painter) const {
    QString years = QString::number(birthYear) + "--" + QString::number(deathYear);
    QFontMetrics fm=painter->fontMetrics();
    return fm.horizontalAdvance(years);
}
void gPerson::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){


    double topMargin = 10.0;
    double lineHeight = 15.0;
    QColor mycolor, oldcolor;
    painter->setFont(*myFont);

    QColor Orange;
    Orange.setRgb(255,153,51);


    QPen pen(Qt::black,1);
    painter->setPen(pen);

    QString Profession =  Profession1();
    if (Profession ==  "linguist"){
        mycolor = Orange;
    }
    else if (Profession == "sociologist"){
             mycolor = Qt::green;
    }
    else if (Profession == "philosopher" || Profession == "Phil-psych"){
            mycolor = Qt::yellow;
     }
    else if (Profession == "psychologist"){
           mycolor = Qt::cyan;
     }
    else{
        mycolor = Qt::magenta;
    }
    if (hasFocus()){
        painter->setPen(Qt::black);
        mycolor = Qt::red;
        //pen.setWidth(5);
    }

    if (!visible){
        //oldcolor = painter->brush().color();
        mycolor =  QColor(255, 255, 255, .4);
        pen.setStyle(Qt::DotLine);
        pen.setColor(Qt::gray);
        painter->setPen(pen);
    }

    //painter->setBrush(brush);
    QString name = firstName + " "  + lastName;
    QString years = QString::number(birthYear) + "--" + QString::number(deathYear);
    float namewidth = GetNameWidth(painter);
    float datewidth = GetDatesWidth(painter);

    if (datewidth > namewidth){
        width = datewidth;
    }else{
        width = namewidth;
    }

    float interline = 2.0;
    height = 2.0 * GetTextHeight(painter) + interline  + 2.0 * margin;



    QRectF personrect(-1.0 * margin,0,width + 2.0 * margin, height);


    QLinearGradient gradient(personrect.topLeft(),personrect.bottomRight());
    gradient.setColorAt(0,Qt::white);
    gradient.setColorAt(1,mycolor);
    //QBrush brush(gradient);

    QBrush brush;
    //brush.setStyle(gradient);




    // THIS IS THE USUAL CODE
    painter->fillRect(personrect,mycolor);

    if(hasFocus()){
        painter->fillRect(personrect, brush);
    } else{
        painter->fillRect(personrect,gradient);
    }

    painter->drawRect(personrect);

    QRectF rect(0,0,namewidth,20);
    QRectF boundingRect1;
    painter->drawText(rect ,Qt::AlignHCenter,  name ,  & boundingRect1);

    QRectF boundingRect2 (boundingRect1);
    rect.moveTo(0,lineHeight);
    painter->drawText(rect, Qt::AlignHCenter,years, &boundingRect2);
    personBoundingRect.setCoords(personrect.left(), personrect.top(), personrect.right(), personrect.bottom() );
    height = personBoundingRect.height();

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
    json["profession1"] = profession1;
    json["visible"] = visible;
    json["limbo"] = limbo;




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
    profession1 = json["profession1"].toString();
    limbo = json["limbo"].toBool();
    visible = json["visible"].toBool();


}
void gPerson::rememberPos(QPointF point){
    xpos = point.x();
    ypos = point.y();
}

void gPerson::ShowSelectedLinkSet(){
  if (selectedLinkSet == "top"){
     if (selectedLink){
         if (topLinks.contains(selectedLink)){
             selectedLink->setSelected();
         }
     }else{   // this is temporary --- ultimately we should check all four sides for which is selected.
         if (topLinks.size() > 0){
             topLinks.at(0)->setSelected();
             topLinks.at(0)->update();
             SetSelectedLink(topLinks.at(0));
         }
     }
  }
}
void gPerson::UnselectAllLinks(){
    if (selectedLink){
        selectedLink->setUnSelected();
        selectedLink->update();
    }
    selectedLink = NULL;
}
