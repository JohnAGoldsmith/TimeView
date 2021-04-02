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
#include <QJsonArray>
#include <QJsonObject>
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
    margin = 15;
    personBoundingRect.setCoords(-1.0 * margin, 0, width + 2*margin, height+2*margin );
    xpos = 0;  // used only in input to program
    selectedLink = NULL;
    grayed = false;
}

// constructor used with .csv  file and Widget for adding person by hand
gPerson::gPerson(QStringList  data){
    setFlags(QGraphicsItem::ItemIsFocusable | QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
    float Xscale = 2.0;
    float Yscale = 20.0;
    key = "no_key";
    float topPosition = 2000.0;
    width = 250; // this isn't right, it should be dynamic and based on relevant data; but that's not available till after Paint();
    height = 70;
    myFont = new  QFont("Times", 12);
    xpos = 0;               // it will be set the first time it is add to scene, using the toppoint on the scene.
    birthYear = 0;
    deathYear = 0;
    margin = 15;
    personBoundingRect.setCoords(-1.0 * margin, 0, width + 2*margin, height+ 2*margin);

    firstName = data[1];
    lastName = data[2];

    birthYear = data[3].toInt();
    if (data[4].length() > 0){
        deathYear = data[4].toInt();
    } else {
        deathYear = 0;
        //qDebug() << lastName << "Zero death year" ;
    }

    x_fromspreadsheet = data[5].toFloat();
    xpos = Xscale *  x_fromspreadsheet;

    ypos = 0.0;
    if( data.length() > 6){
        if (data[6].length() > 0){
            y_fromspreadsheet = data[6].toFloat();
            if (y_fromspreadsheet > 0){
                ypos = y_fromspreadsheet;
            }
        }
    }
    if (ypos == 0.0){
        if (birthYear > 0){
            ypos = Yscale * (topPosition - birthYear);
       } else{
            ypos = 0.0;
        }
    }

    key = "";
    if (data.length() > 7){
        if (data[7].length() > 0){
            key = data[7];
        }
    }
    if (key.length() == 0){
        key = lastName;
    }
    if (data.length() > 8){
        height = data[8].toFloat();
    }
    if (data.length() > 9) {
        width = data[9].toFloat();
    }
    if (data.length() > 10){
        profession1 = data[10];
    }
    selectedLink = NULL;
    grayed = false;
}
// constructor used with legacy .csv  file only
gPerson::gPerson(bool dummy, QStringList  data){
    setFlags(QGraphicsItem::ItemIsFocusable | QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
    float topPosition = 2000.0;
    float Xscale = 8.0;
    float Yscale = 20.0;
    key = "no_key";
    width = 250; // this isn't right, it should be dynamic and based on relevant data; but that's not available till after Paint();
    height = 70;
    myFont = new  QFont("Times", 12);
    xpos = 0;               // it will be set the first time it is add to scene, using the toppoint on the scene.
    birthYear = 0;
    deathYear = 0;
    margin = 15;
    personBoundingRect.setCoords(-1.0 * margin, 0, width + 2*margin, height+ 2*margin);

    firstName = data[1];
    lastName = data[2];
    birthYear = data[3].toInt();
    if (birthYear == 0){
        //qDebug() << lastName << "zero birth year a";
    }
    if (data[4].length() > 0){
        deathYear = data[4].toInt();
    } else {
        deathYear = 0;
        //qDebug() << lastName << "Zero death year" ;
    }
    x_fromspreadsheet = data[5].toFloat() * Xscale;
    xpos =x_fromspreadsheet * Xscale;
    ypos = Yscale * (topPosition - birthYear);

    if (data.size() > 6){
        profession1 = data[6];
    }
    if (data.size() >= 8 && data[7].length() > 0){
        key = data[7];
    } else {
        key = data[2];
    }

    selectedLink = NULL;
    grayed = false;
}
gPerson::~gPerson(){

}
void gPerson::SetKey2Link(QString key, cLink *link){
    myKeys2Links[key] = link;
}
void gPerson::AppendTopLinkKey(QString key){
    topLinkKeys.append(key);
}
void gPerson::AppendLeftLinkKey(QString key){
    leftLinkKeys.append(key);
}
void gPerson::AppendBottomLinkKey(QString key){
    bottomLinkKeys.append(key);
}
void gPerson::AppendRightLinkKey(QString key){
    rightLinkKeys.append(key);
}
QPointF gPerson::GetMemoryOfScreenPosition(){
    return QPointF(xpos,ypos);
}
QString gPerson::export2CSV(){
    QStringList temp;
    temp.append("P");
    temp.append(firstName);
    temp.append(lastName);
    temp.append(QString::number(birthYear));
    temp.append(QString::number(deathYear));
    temp.append(QString::number(pos().x()));
    temp.append(QString::number(pos().y()));
    if ( key != lastName){
        temp.append(key);
    } else {
        temp.append(lastName);
    }
    temp.append(QString::number(height));
    temp.append(QString::number(width));
    temp.append(profession1);
    return temp.join(",");
}


void gPerson::AppendLinkAndResortEdges(cLink *link) {
    //AppendLinkAndAddLinkKeyToSide(link);
    myLinks.append(link);
    AddLinkKeyToSide(link);
    SortLinksOnEachSide();
}
void gPerson::AppendLinkAndAddLinkKeyToSide(cLink *link) {
    myLinks.append(link);
    if (link->FromPerson() == this){
        if (link->GetPositionOnFromPerson() == "Top"){
            topLinkKeys.append(link->getKey());
            //topLinks.append(link);
        }
        else if (link->GetPositionOnFromPerson() == "Bottom"){
            bottomLinkKeys.append(link->getKey());
            bottomLinks.append(link);
        }
        else if (link->GetPositionOnFromPerson() == "Left"){
            leftLinkKeys.append(link->getKey());
            leftLinks.append(link);
        }
        else if (link->GetPositionOnFromPerson() == "Right"){
            rightLinkKeys.append(link->getKey());
            rightLinks.append(link);
        }
    }
    if (link->ToPerson()==this){
        if (link->GetPositionOnToPerson() == "Top"){
            //topLinks.append(link);
            topLinkKeys.append(link->getKey());
        }
        else if (link->GetPositionOnToPerson() == "Bottom"){
            bottomLinks.append(link);
            bottomLinkKeys.append(link->getKey());
        }
        else if (link->GetPositionOnToPerson() == "Left"){
            leftLinks.append(link);
            leftLinkKeys.append(link->getKey());
        }
        else if (link->GetPositionOnToPerson() == "Right"){
            rightLinks.append(link);
            rightLinkKeys.append(link->getKey());
        }
    }
}
void gPerson::AddLinkKeyToSide(cLink* link){
    if (link->FromPerson() == this || link->ToPerson() == this){
        if (link->GetPositionOnFromPerson() == "Top"){
            topLinkKeys.append(link->getKey());
        }
        else if (link->GetPositionOnFromPerson() == "Bottom"){
            bottomLinkKeys.append(link->getKey());
        }
        else if (link->GetPositionOnFromPerson() == "Left"){
            leftLinkKeys.append(link->getKey());
        }
        else if (link->GetPositionOnFromPerson() == "Right"){
            rightLinkKeys.append(link->getKey());
        }
    }
}
//void gPerson::AppendTopLink(cLink * link){
    //topLinks.append(link);
//}
void gPerson::AppendBottomLink(cLink * link) {
    bottomLinks.append(link);
}

QRectF gPerson::boundingRect() const  {
    return personBoundingRect;
}

void gPerson::focusInEvent(QFocusEvent * event){
    QGraphicsItem::focusInEvent(event);
}
void gPerson::focusOutEvent(QFocusEvent * event){LastName();
    UnselectAllLinks();
    QGraphicsItem::focusOutEvent(event);
}

void gPerson::mousePressEvent(QGraphicsSceneMouseEvent * event){
    QGraphicsItem::mousePressEvent(event);
}
void gPerson::keyPressEvent (QKeyEvent * event){
    //qDebug() << "Person key event" << event->text();
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
              //qDebug() << "gperson k key";
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
              //qDebug() << "gperson 170 moving selected link to right ";
              //int index(topLinks.indexOf(selectedLink));
              int index(topLinkKeys.indexOf(selectedLinkKey));
              if (index >= 0 && index < topLinkKeys.size()-1){
                  qDebug() << 173 << index;
                  topLinkKeys.swapItemsAt(index,index+1);
                  qDebug() << 175;
                  SortLinksOnEachSide();
                  Scene()->update();
              }
          }
      }
   }
   else if (event->key() == Qt::Key_9){
      if (selectedLink){
          if (myLinks.size() > 1){
              //qDebug() << "gperson 185 moving selected link to LEFT ";
              int index(topLinkKeys.indexOf(selectedLinkKey));
              if (index > 0){
                  //qDebug() << 188 << index;
                  topLinkKeys.swapItemsAt(index,index-1);
                  //qDebug() << 190;
                  SortLinksOnEachSide();
                  Scene()->update();
              }
          }
      }
   }
   /*    shorten the first leg of the link       */
   else if (event->key() == Qt::Key_1){
      if (selectedLink){
          if (myLinks.size() > 1){
              //qDebug() << "found leg";
              //qDebug() << "Shorten the first leg of this link";
              int index(topLinkKeys.indexOf(selectedLinkKey));
              //qDebug() << "index: " <<index;
              if (index >= 0){
                  //qDebug() << "going to function to shorten";
                  selectedLink->shortenProportion1();
                  //qDebug() << "proportion" << selectedLink->GetProportion1();
                  Scene()->update();
              }
          }
      }
   }
   /*    lengthen the first leg of the link       */
   else if (event->key() == Qt::Key_2){
      if (selectedLinkKey.size() > 0){
          cLink * selectedLink = Key2Link(selectedLinkKey);
          selectedLink->lengthenProportion1();
          //qDebug() << selectedLink->GetProportion1();
          Scene()->update();
           }
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
   QGraphicsItem::keyPressEvent(event);

}
void gPerson::mouseMoveEvent(QGraphicsSceneMouseEvent * event){ // this doesn't get called with json, but it does with csv!!
    foreach (cLink* link, * GetLinks()){
        //if (link->GPersonFrom() == this){
        if (link->getFromKey() == Key()){
           link->setPos(scenePos());
           link->update();
        }
    }
    //qDebug() << "person: mouse move detected";
    QGraphicsItem::mouseMoveEvent(event);
}



void gPerson::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
    if (event->modifiers()==Qt::ShiftModifier){
        //if (!visible)
        //    return;
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

    double lineHeight = 25.0;
    double topmargin = 10.0;
    QColor mycolor;
    QFont localFont = *myFont;
    localFont.setCapitalization(QFont::SmallCaps);
    painter->setFont(localFont);
    QPen pen(Qt::black,3);
    QHash<QString, QColor> colorHash;
    QHash<QString, QPixmap*> pixmapHash;

    /*  Set up ColorHash             */
    colorHash["linguist"] = QColor(244,153,51);
    colorHash["sociologist"] = Qt::green;
    colorHash["philosopher"] = Qt::yellow;
    colorHash["Phil-psych"] = Qt::yellow;
    colorHash["psychologist"] = Qt::cyan;
    colorHash["other"] = Qt::magenta;
    colorHash["focus"] = Qt::red;
    colorHash["grayed"] =  QColor(255, 255, 255, 1);

    /*     Set up   pixmap                  */

    /*
    cScene * thisScene = dynamic_cast<cScene*>(scene);
    QHash<QString,QPixmap*> * pixmaps = thisScene->Pixmaps();
    QPixmap * pixmap(NULL);

if (pixmaps->contains("apricot")){
       pixmapHash["linguist"] = pixmaps->value("apricot");
    } else pixmapHash["linguist"] = NULL;
    if (pixmaps->contains("darkredwood")){
       pixmapHash["sociologist"] = pixmaps->value("darkredwood");
       //pen.setColor(Qt::white);
    } else pixmapHash["sociologist"] = NULL;
    if (pixmaps->contains("blue")){
       pixmapHash["philosopher"] = pixmaps->value("blue");
    } else pixmapHash["philosopher"] = NULL;
    if (pixmaps->contains("blue")){
       pixmapHash["Phil-psych"] = pixmaps->value("blue");
    } else pixmapHash["Phil-psych"] = NULL;
    if (pixmaps->contains("lightbrown")){
       pixmapHash["psychologist"] = pixmaps->value("lightbrown");
    } else pixmapHash["psychologist"] = NULL;
    if (pixmaps->contains("yellow")){
       pixmapHash["other"] = pixmaps->value("yellow");
    } else pixmapHash["other"] = NULL;
    if (pixmaps->contains("red")){
       pixmapHash["focus"] = pixmaps->value("red");
    } else pixmapHash["focus"] = NULL;

    if (hasFocus()) { pixmap = NULL; }
    else if (pixmapHash.contains(Profession1())){
        pixmap = pixmapHash[Profession1()];
    } else { pixmap = NULL; }


    //             Set up pen colors
    if (pixmap== pixmaps->value("darkredwood")){
        pen.setColor(Qt::white);
    }
    */

    if (hasFocus())  { mycolor = colorHash["focus"]; }
    else if (colorHash.contains(Profession1())) {
        mycolor = colorHash[Profession1()];
    } else { mycolor = colorHash["other"]; }


    if (grayed){
        pen.setStyle(Qt::DotLine);
        pen.setColor(Qt::gray);
        painter->setPen(pen);
    }
    painter->setPen(pen);

    QString name = firstName + " "  + lastName;
    QString years = QString::number(birthYear) + "--" + QString::number(deathYear);
    float namewidth = GetNameWidth(painter);
    float datewidth = GetDatesWidth(painter);
    if (datewidth > namewidth){
        width = datewidth;
    } else{
        width = namewidth;
    }
    width += 2.0 * margin;
    float interline = 2.0;
    height = 2.0 * GetTextHeight(painter) + interline  + 1.0 * margin;
    QRectF fullrect(0,0,width, height);

    /*
    QLinearGradient gradient(personrect.topLeft(),personrect.bottomRight());
    gradient.setColorAt(0,mycolor);
    gradient.setColorAt(0.5,Qt::white);
    gradient.setColorAt(1,mycolor);
    painter->fillRect(personrect,mycolor);
    if(hasFocus()){
        painter->fillRect(personrect, brush);
    } else{
        painter->fillRect(personrect,gradient);
    }
    if (pixmap) {
        if (! grayed ){
            painter->drawPixmap(personrect,*pixmap,QRectF(0,0,50,50));
        }
    }
    */

    painter->drawRect(fullrect);
    painter->fillRect(fullrect, mycolor);
    QRectF boundingRect1;
    QRectF smallrect(0,0,width, GetTextHeight(painter)+ margin);
    painter->drawText(smallrect ,Qt::AlignHCenter,  name ,  & boundingRect1);



    QRectF boundingRect2 (boundingRect1);
    smallrect.moveTo(0,lineHeight);
    painter->drawText(smallrect, Qt::AlignHCenter,years, &boundingRect2);
    personBoundingRect = fullrect;
    height = personBoundingRect.height();
}

void gPerson::ShiftLink(){
    //QList<cLink*> * thisList = GetTopLinks();
    QStringList * thisList = GetTopLinkKeys();
    if (thisList->size() < 2)
        return;
    if (!selectedLink)
        return;
    int thisindex;
    thisindex = thisList->indexOf(selectedLinkKey);
    if (thisindex < 0)
        return;
    if (thisindex==0){
        thisList->move(0,thisList->size()-1);
    }else{
        thisList->move(thisindex,thisindex-1);
    }

}
void gPerson::SortLinksOnEachSide(){
    float boxwidth = 100.0; // should be GetNameWidth();
    float delta = (boxwidth * 0.5) / GetTopLinkKeys()->size();

    //QList<cLink*> * thisList = GetTopLinks();
    QStringList  * thisList = GetTopLinkKeys();
    int LS = thisList->size();
    if (LS > 1){
        int i = 0;
        float startingPoint;
        startingPoint =  -1.0 * ( (LS/2.0  ) * delta ) ;
        foreach (QString linkKey, *thisList){
            cLink* thislink = Key2Link(linkKey) ;
            thislink->TopOffset(startingPoint + i * delta);
            i++;
        }
    }
    delta = (boxwidth * 0.5) / GetBottomLinks()->size();
    thisList = GetBottomLinkKeys();
    LS = thisList->size();
    if (LS > 1){
        int i = 0;
        float startingPoint;
        startingPoint =  -1.0 * ( (LS/2.0  ) * delta ) ;
        foreach (QString linkKey, * thisList){
            cLink* thisLink = Key2Link(linkKey);
            thisLink->BottomOffset(startingPoint + i * delta);
            i++;
        }
    }
}

void gPerson::write(QJsonObject & json) const {
    json["key"] = key;
    json["type"] = "person";
    json["firstName"] = firstName;
    json["lastName"] = lastName;
    json["xpos"] = pos().x();
    json["ypos"] = pos().y();
    json["height"] = height;

    json["birthYear"] = birthYear;
    json["deathYear"] = deathYear;
    json["profession1"] = profession1;
    json["visible"] = isVisible();
    json["grayed"] = grayed;

    QJsonArray JsontopLinks;
    for  (int i =0; i < topLinkKeys.size();  i++){
        QString key = topLinkKeys[i];
        QJsonValue json(key);
        JsontopLinks.append(json);
    }
    json["topLinkKeys"] = JsontopLinks;

    QJsonArray JsonbottomLinks;
    for  (int i =0; i < bottomLinks.size();  i++){
        QString key = bottomLinkKeys[i];
        QJsonValue json(key);
        JsonbottomLinks.append(json);
    }
    json["bottomLinkKeys"] = JsonbottomLinks;

    QJsonArray JsonleftLinks;
    for  (int i =0; i < leftLinks.size();  i++){
        QString key = leftLinkKeys[i];
        QJsonValue json(key);
        JsonbottomLinks.append(json);
    }
    json["leftLinkKeys"] = JsonleftLinks;

    QJsonArray JsonrightLinks;
    for  (int i =0; i < rightLinks.size();  i++){
        QString key = rightLinkKeys[i];
        QJsonValue json(key);
        JsonrightLinks.append(json);
    }
    json["rightLinkKeys"] = JsonrightLinks;

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

    if (json.contains("topLinkKeys") && json["topLinkKeys"].isArray()){
        QJsonArray linkKeyArray = json["topLinkKeys"].toArray();
        for (int j = 0; j < linkKeyArray.size(); j++){
            AppendTopLinkKey(linkKeyArray[j].toString());
        }
    }
    if (json.contains("rightLinkKeys") && json["rightLinkKeys"].isArray()){
        QJsonArray linkKeyArray = json["rightLinkKeys"].toArray();
        for (int j = 0; j < linkKeyArray.size(); j++){
            AppendRightLinkKey(linkKeyArray[j].toString());
        }
    }
    if (json.contains("bottomLinkKeys") && json["bottomLinkKeys"].isArray()){
        QJsonArray linkKeyArray = json["bottomLinkKeys"].toArray();
        for (int j = 0; j < linkKeyArray.size(); j++){
            AppendBottomLinkKey(linkKeyArray[j].toString());
        }
    }
    if (json.contains("leftLinkKeys") && json["leftLinkKeys"].isArray()){
        QJsonArray linkKeyArray = json["leftLinkKeys"].toArray();
        for (int j = 0; j < linkKeyArray.size(); j++){
            AppendLeftLinkKey(linkKeyArray[j].toString());
        }
    }
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
