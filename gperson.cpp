#include <QGraphicsItem>
#include <QGraphicsTextItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsProxyWidget>
#include <QPainter>
#include <QStylePainter>
#include <QGradient>
#include <QDebug>
#include <QColor>
#include <QPixmap>
#include <QImage>
#include <QGraphicsDropShadowEffect>
#include <QFontMetrics>
#include <QPushButton>
#include <QJsonArray>
#include <QKeyEvent>
#include "gperson.h"
#include "cscene.h"
#include "clink.h"
#include "cview.h"


// constructor used with Json input //
gPerson::gPerson(){
 setFlags(QGraphicsItem::ItemIsFocusable | QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
 myFont = new  QFont("Times", 12);

 width = 250; // this isn't right, it should be dynamic and based on relevant data; but that's not available till after Paint();
 height = 70;
 margin = 5;
 visible=true;
 personBoundingRect.setCoords(-1.0 * margin, 0, width + 2*margin, height+2*margin );
 xpos = 0;  // it will be set the first time it is add to scene, using the toppoint on the scene.
 selectedLink = NULL;
 grayed = false;
}

// constructor used with .csv  file and Widget for adding person by hand
gPerson::gPerson(QStringList  data){
    setFlags(QGraphicsItem::ItemIsFocusable | QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);

    key = "no_key";
    width = 250; // this isn't right, it should be dynamic and based on relevant data; but that's not available till after Paint();
    height = 70;
    myFont = new  QFont("Times", 12);
    xpos = 0;               // it will be set the first time it is add to scene, using the toppoint on the scene.
    birthYear = 0;
    deathYear = 0;
    margin = 5;
    personBoundingRect.setCoords(-1.0 * margin, 0, width + 2*margin, height+ 2*margin);

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
    if (data.size() >= 8 && data[7].length() > 0){
        key = data[7];
    } else {
        key = data[2];
    }
    visible=true;
    selectedLink = NULL;
    grayed = false;
}

gPerson::~gPerson(){

}
QString gPerson::export2CSV(){
  QStringList temp;
  temp.append("P");
  temp.append(firstName);
  temp.append(lastName);
  temp.append(QString::number(birthYear));
  temp.append(QString::number(deathYear));
  temp.append(QString::number(xpos));
  temp.append(profession1);
  if ( key != lastName){
      temp.append(key);
  }
  return temp.join(",");
}


void gPerson::AppendLink(cLink *link) {
    myLinks.append(link);
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

void gPerson::AppendTopLink(cLink * link){
    topLinks.append(link);
}
void gPerson::AppendBottomLink(cLink * link) {
    bottomLinks.append(link);
}

QRectF gPerson::boundingRect() const  {
    return personBoundingRect;
}



void gPerson::focusInEvent(QFocusEvent * event){

    //qDebug() << "124 I have focus" << LastName();
    QGraphicsItem::focusInEvent(event);
}
void gPerson::focusOutEvent(QFocusEvent * event){
    qDebug() << "gperson line 124 leaving focus"<< LastName();
    UnselectAllLinks();
    QGraphicsItem::focusOutEvent(event);
}

void gPerson::mousePressEvent(QGraphicsSceneMouseEvent * event){
    //qDebug() << "gPerson mousepressevent clicked" << "x" << event->scenePos().x() << "y" << event->scenePos().y();
    QGraphicsItem::mousePressEvent(event);
    //update();
}

void gPerson::keyPressEvent (QKeyEvent * event){
    //qDebug() << "person 143" << event->text();


   if (event->key() == Qt::Key_L ){
      if (!selectedLink){
          if (myLinks.size()){
             selectedLink = myLinks.first();
             selectedLink->setChosen();
             Scene()->update();
          }
      }    
   }
   else if (event->key() == Qt::Key_K){
        if (selectedLink){
          if (myLinks.size() > 1){
              qDebug() << "gperson k key";
              selectedLink->setUnChosen();
              int index = myLinks.indexOf(selectedLink);
              if (index == myLinks.size()-1 ){
                  index = 0;
              } else{
                  index += 1;
              }
              selectedLink = myLinks.at(index);
              selectedLink->setChosen();
              Scene()->update();             
          }
      }
   }
   else if (event->key() == Qt::Key_0){
      if (selectedLink){
          if (myLinks.size() > 1){
              qDebug() << "gperson 170 moving selected link to right ";
              int index(topLinks.indexOf(selectedLink));
              if (index >= 0 && index < topLinks.size()-1){
                  qDebug() << 173 << index;
                  topLinks.swapItemsAt(index,index+1);
                  qDebug() << 175;
                  SortLinks();
                  Scene()->update();
              }
          }
      }
   }
   else if (event->key() == Qt::Key_9){
      if (selectedLink){
          if (myLinks.size() > 1){
              qDebug() << "gperson 185 moving selected link to LEFT ";
              int index(topLinks.indexOf(selectedLink));
              if (index > 0){
                  qDebug() << 188 << index;
                  topLinks.swapItemsAt(index,index-1);
                  qDebug() << 190;
                  SortLinks();
                  Scene()->update();
              }
          }
      }
   }
   /*    shorten the first leg of the link       */
   else if (event->key() == Qt::Key_1){
      if (selectedLink){
          if (myLinks.size() > 1){
              qDebug() << "found leg";
              qDebug() << "Shorten the first leg of this link";
              int index(topLinks.indexOf(selectedLink));
              qDebug() << "index: " <<index;
              if (index >= 0){
                  qDebug() << "going to function to shorten";
                  selectedLink->shortenProportion1();
                  qDebug() << "proportion" << selectedLink->GetProportion1();
                  Scene()->update();
              }
          }
      }
   }
   /*    lengthen the first leg of the link       */
   else if (event->key() == Qt::Key_2){
      if (selectedLink){
          if (myLinks.size() > 1){
              int index(topLinks.indexOf(selectedLink));
              if (index >= 0){
                  selectedLink->lengthenProportion1();
                  qDebug() << selectedLink->GetProportion1();
                  Scene()->update();
              }
          }
      }

   QGraphicsItem::keyPressEvent(event);
}
/*           Replace person on correct y-coor for their birthyear         */
   else if (event->key() == Qt::Key_B){
       int old_x = x();
       int new_y = Scene()->ConvertYearToYcoor(birthYear);
       setPos(old_x, new_y  );
       rememberPos(QPointF(old_x,new_y));
       Scene()->update();
       QGraphicsItem::keyPressEvent(event);
   }
}
void gPerson::mouseMoveEvent(QGraphicsSceneMouseEvent * event){ // this doesn't get called with json, but it does with csv!!
    foreach (cLink* link, * GetLinks()){
        if (link->GPersonFrom() == this){
           link->setPos(scenePos());
           link->update();
        }
    }
    QGraphicsItem::mouseMoveEvent(event);
}



void gPerson::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
    if (event->modifiers()==Qt::ShiftModifier){
        if (!visible)
            return;
        if (!grayed){
          grayed = true;
          update();
          foreach(cLink* link, myLinks){
             link->setGrayed(true);
             link->update();
          }
        } else {
            grayed = false;
            update();
            foreach(cLink * link, myLinks){
                link->setGrayed(false);
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

    double lineHeight = 15.0;
    QColor mycolor;
    painter->setFont(*myFont);

    cScene * thisScene = dynamic_cast<cScene*>(scene);

    QGraphicsScene * j = scene;
    QHash<QString,QPixmap*> * pixmaps = thisScene->Pixmaps();
    QPixmap * pixmap(NULL);

    QColor Orange;
    Orange.setRgb(255,153,51);

    QPen pen(Qt::black,1);
    painter->setPen(pen);

    if (! visible){
        return;
    }
    QString Profession =  Profession1();
    if (Profession ==  "linguist"){
        mycolor = Orange;
        if (pixmaps->contains("apricot")){
           pixmap = pixmaps->value("apricot");
        }
    }
    else if (Profession == "sociologist"){
             mycolor = Qt::green;
             if (pixmaps->contains("darkredwood")){
                pixmap = pixmaps->value("darkredwood");
                pen.setColor(Qt::white);
                painter->setPen(pen);
             }
    }
    else if (Profession == "philosopher" || Profession == "Phil-psych"){
            mycolor = Qt::yellow;
            if (pixmaps->contains("blue")){
               pixmap = pixmaps->value("blue");
            }
     }
    else if (Profession == "psychologist"){
           mycolor = Qt::cyan;
           if (pixmaps->contains("lightbrown")){
              pixmap = pixmaps->value("lightbrown");
           }
     }
    else{
        mycolor = Qt::magenta;
        if (pixmaps->contains("yellow")){
           pixmap = pixmaps->value("yellow");
           //pen.setColor(Qt::white);
           painter->setPen(pen);
        }    }
    if (hasFocus()){
        painter->setPen(Qt::black);
        mycolor = Qt::red;
        if (pixmaps->contains("red")){
           pixmap = pixmaps->value("red");
        }
    }

    if (grayed){
        //oldcolor = painter->brush().color();
        mycolor =  QColor(255, 255, 255, 1);
        pen.setStyle(Qt::DotLine);
        pen.setColor(Qt::gray);
        painter->setPen(pen);
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

    float interline = 2.0;
    height = 2.0 * GetTextHeight(painter) + interline  + 2.0 * margin;
    QRectF personrect(-1.0 * margin,0,width + 2.0 * margin, height);

    QLinearGradient gradient(personrect.topLeft(),personrect.bottomRight());
    gradient.setColorAt(0,mycolor);
    gradient.setColorAt(0.5,Qt::white);
    gradient.setColorAt(1,mycolor);


    QBrush brush;

    painter->fillRect(personrect,mycolor);

    if(hasFocus()){
        painter->fillRect(personrect, brush);
    } else{
        painter->fillRect(personrect,gradient);
    }

    painter->drawRect(personrect);

    if (! grayed){
       painter->drawPixmap(personrect,*pixmap,QRectF(0,0,50,50));
    }



    QRectF rect(0,0,namewidth,20);
    QRectF boundingRect1;
    painter->drawText(rect ,Qt::AlignHCenter,  name ,  & boundingRect1);

    QRectF boundingRect2 (boundingRect1);
    rect.moveTo(0,lineHeight);
    painter->drawText(rect, Qt::AlignHCenter,years, &boundingRect2);
    personBoundingRect.setCoords(personrect.left() - 1, personrect.top() -1, personrect.right()+1, personrect.bottom() + 1);
    height = personBoundingRect.height();




}

void gPerson::ShiftLink(){
    QList<cLink*> * thisList = GetTopLinks();
    if (thisList->size() < 2)
        return;
    if (!selectedLink)
        return;
    int thisindex;
    thisindex = thisList->indexOf(selectedLink);
    if (thisindex < 0)
        return;
    if (thisindex==0){
        thisList->move(0,thisList->size()-1);
    }else{
        thisList->move(thisindex,thisindex-1);
    }

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
    json["grayed"] = grayed;

    QJsonArray linkArray;
    foreach ( cLink * link, myLinks){
        QJsonObject linkObject;
        link->write(linkObject);
        linkArray.append(linkObject);
    }
    json["Links"] = linkArray;

    QJsonArray linkkeyArray;
    foreach ( cLink * link, myLinks){
        QJsonValue linkkeyValue(link->getKey());
        linkkeyArray.append(linkkeyValue);
    }
    json["LinkKeys"] = linkkeyArray;

    QJsonArray JsontopLinks;
    for  (int i =0; i < topLinks.size();  i++){
        QString key = topLinks[i]->getKey();
        QJsonValue json(key);
        JsontopLinks.append(json);
    }
    json["topLinks"] = JsontopLinks;

    QJsonArray JsonbottomLinks;
    for  (int i =0; i < bottomLinks.size();  i++){
        QString key = bottomLinks[i]->getKey();
        QJsonValue json(key);
        JsonbottomLinks.append(json);
    }
    json["bottomLinks"] = JsonbottomLinks;











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
    grayed = json["grayed"].toBool();
    visible = json["visible"].toBool();






}
void gPerson::rememberPos(QPointF point){
    xpos = point.x();
    ypos = point.y();
}


void gPerson::UnselectAllLinks(){
    if (selectedLink){
        selectedLink->setUnChosen();
        selectedLink->update();
    }
    selectedLink = NULL;
}
