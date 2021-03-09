#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QGraphicsWidget>
#include <QGraphicsProxyWidget>
#include <QPushButton>
#include <QGraphicsDropShadowEffect>
#include "data.h"
#include "clink.h"
#include "cscene.h"

cData::cData()
{
   topPosition = 1950;
}
void cData::Clear(){

    dataPersons.clear();
    graphicalPersons.clear();
    Links.clear();
    Limbo.clear();
    Key2graphicalPerson.clear();
    tempLines.clear();

}
cLink* cData::getLink(QString key){
    if (hashLinks.contains(key)){
        return hashLinks[key];
    }
    return NULL;
}
cLink* cData::CheckAndCreateNewLink(QString key){
    if (hashLinks.contains(key)){
        return NULL;
    }
    cLink * link = new cLink();
    hashLinks[key] = link;
    Links.append(link);
    return link;

}
void cData::A_ReadCSV(QString filename)
{

    QFile file(filename);
    qDebug() << QDir::currentPath();
    if (! file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream stream(&file);
    QStringList data;
    QString separator(",");
    int topPosition = 1950;
    while (stream.atEnd() == false)
    {
        QString line = stream.readLine();
        tempLines << line;
    }

    file.close();
    return;
}

void cData::A_analyzeData(){
    gPerson * gperson, * fromgperson, * togperson;
    cLink * l;
    QString outfileName, fromKey, toKey;
    foreach (QString line1 , tempLines){
       QStringList line = line1.split(",");
       if (line.size() < 1)
           continue;
       if (line[0][0]== "!") {
              outfileName = line[1];
       } else {
           if (line[0][0] == "#"){
               continue;
           } else {
               if (line[0] == "P"){
                   gperson = B_CreateGraphicalPerson(line);
               } else {
                   if (line[0] == "L"){
                        l = new cLink(line);
                        Links.append(l);
                   }
               }
           }
       }
    }
    B_AddGPersonPtrsToLinks();
}

gPerson* cData::B_CreateGraphicalPerson(QStringList line){
    QString key;
    if (line.size() >= 8 && line[7].length() > 0){
        key = line[7];
    } else {
        key = line[2];
    }
    if ( Key2PersonHashContains(key) ) {
       qDebug() << "cData:  collision of graphicalPerson keys" << key << "No graphical unit created.";
       return NULL;
    };
    gPerson* gperson = new gPerson(line);
    Key2graphicalPerson[key]= gperson;
    graphicalPersons.append(gperson);
}

void cData::B_AddGPersonPtrsToLinks(){
    QString fromKey, toKey;
    gPerson * fromgperson, *togperson;
    foreach (cLink* link, Links){
        fromKey = link->getFromKey();
        toKey = link->getToKey();
        if (! Key2PersonHashContains(fromKey)){
            qDebug() << "Link containing "<<fromKey << "and" << toKey << "cannot be made;"<<fromKey << "is missing.";
            continue;
        }
        if (! Key2PersonHashContains(toKey)){
            qDebug() << "Link containing "<<fromKey << "and" << toKey << "cannot be made;"<<toKey << "is missing.";
            continue;
        }
        fromgperson = getPersonFromKey(fromKey);
        togperson = getPersonFromKey(toKey);
        link->attachGraphicalPersons(fromgperson,togperson);
    }
}


void cData::A_sendPersonsAndLinksToScene(cScene* scene){
    gPerson * gPerson1, * gPerson2;

    foreach (gPerson * gperson, graphicalPersons){
        scene->addItem(gperson);
        gperson->Scene(scene); // why is this necessary? Why can't I get this from the gperson?
        float new_x = gperson->X_fromspreadsheet() * scene->ScaleFactor();
        float new_y = scene->TimeScale() * ( scene->TopPosition() - gperson->BirthYear());
        gperson->setPos(new_x, new_y  );
        qDebug() << new_x << new_y <<  "line 140 data";
        gperson->rememberPos(QPointF(new_x,new_y));

    }
    foreach (cLink * link, Links){
        gPerson1 = link->GPersonFrom();
        gPerson2 = link->GPersonTo();
        if ( ! gPerson1 || ! gPerson2 ){
            qDebug() << "Cannot send link to Scene 124 "<<link->display() << "Missing gperson." ;
            continue;
        }
        scene->AddLink(link);
    }
}
void cData::A_sendPersonsAndLinksToSceneJson(cScene* scene){
    gPerson * gPerson1, * gPerson2;

    foreach (gPerson * gperson, graphicalPersons){
        scene->addItem(gperson);
        gperson->Scene(scene); // why is this necessary? Why can't I get this from the gperson?
        gperson->setPos(gperson->Xpos(), gperson->Ypos());  // Was this the problem of the links that didn move?´
        gperson->rememberPos(QPointF(gperson->Xpos(),gperson->Ypos()));
    }
    foreach (cLink * link, Links){
        gPerson1 = link->GPersonFrom();
        gPerson2 = link->GPersonTo();
        if ( ! gPerson1 || ! gPerson2 ){
            qDebug() << "Cannot send link to Scene 124 "<<link->display() << "Missing gperson." ;
            continue;
        }
        scene->AddLink(link);
    }
}


void cData::write(QJsonObject &json) const{
    QJsonArray linkArray;
    foreach (const cLink * link, Links){
       if (link->GPersonTo()){
         QJsonObject linkObject;
         link->write(linkObject);
         linkArray.append(linkObject);
       }
    }
    json["Links"]=linkArray;

    QJsonArray gPersonArray;
    foreach (const gPerson * gp, graphicalPersons){
         QJsonObject gPersonObject;
         gp->write(gPersonObject);
         gPersonArray.append(gPersonObject);
    }
    json["GraphicalPersons"]=gPersonArray;

    return ;
}
void cData::A_ReadJson(QString filename) {
    QFile file (filename);
    if (! file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    cLink * link;
    QByteArray thisData = file.readAll();
    QJsonDocument loadDoc(QJsonDocument::fromJson(thisData));
    QJsonObject rootItem = loadDoc.object(); // this should be the root item;

    if (rootItem.contains("GraphicalPersons") && rootItem["GraphicalPersons"].isArray() ){
        QJsonArray gPersonArray = rootItem["GraphicalPersons"].toArray();
        for (int index = 0; index < gPersonArray.size(); ++index){
            QJsonObject gpObject = gPersonArray[index].toObject();
            QString key1 = gpObject["key"].toString();
            gPerson* gperson = new gPerson();
            gperson->read(gpObject);               /*  this ignores links   */
            Key2graphicalPerson[key1] = gperson;
            graphicalPersons.append(gperson);
            /* The next lines should go into Json-read function, *except* that they call a cData function which creates a new cLink */
            if (gpObject.contains("Links") && gpObject["Links"].isArray() ){
              QJsonArray linkArray = gpObject["Links"].toArray();
              for (int index = 0; index < linkArray.size(); index++){
                 QJsonObject linkObject = linkArray[index].toObject();
                 QString key = linkObject["Key"].toString();
                 link = CheckAndCreateNewLink(linkObject["Key"].toString());
                 if (link){
                    link->read(linkObject);
                 }
                 link = getLink(key);
                 gperson->AppendLink(link);
                 /*   What follows should be impossible    */
                 if (! link->GPersonFrom()){
                    QString fromKey = link->getFromKey();
                    if (Key2PersonHashContains(fromKey)){
                        link->setPersonFrom(Key2graphicalPerson[fromKey]);
                    }
                 }
                 if (! link->GPersonTo() ) {
                    QString toKey = link->getToKey();
                    if (Key2PersonHashContains(toKey)){
                        link->setPersonTo(Key2graphicalPerson[toKey]);
                    }
                 }
              }
            }
            if (gpObject.contains("topLinks")) {
                    if ( gpObject["topLinks"].isArray() )  {
                 QJsonArray toplinkArray = gpObject["topLinks"].toArray();
                 for (int i = 0; i < toplinkArray.size(); i++){
                    QString linkkey = toplinkArray[i].toString();
                    gperson->GetTopLinks()->append( Key2Link(linkkey) );
                   }
                }
            }

            auto effect = new QGraphicsDropShadowEffect();
            effect->setBlurRadius(5);
            effect->setXOffset(15);
            effect->setYOffset(5);
            effect->setColor(QColor("#555555"));
            gperson->setGraphicsEffect(effect);
            }
    }

    if (  rootItem.contains ("Links") && rootItem["Links"].isArray()  ){
       QJsonArray linkArray = rootItem["Links"].toArray();
       for (int index = 0; index < linkArray.size(); ++index){
           QJsonObject linkObject = linkArray[index].toObject();
           cLink * link = CheckAndCreateNewLink(linkObject["Key"].toString());
           if (link){
               link->read(linkObject);
               gPerson* gpersonFrom = Key2graphicalPerson[link->getFromKey()];
               gPerson* gpersonTo   = Key2graphicalPerson[link->getToKey()];
               link->attachGraphicalPersons(gpersonFrom, gpersonTo);
               Links.append(link);
              if (! Key2PersonHashContains(link->getFromKey())){
                 qDebug() << "Link has From keyword that does not exist.";
                 continue;
              }
              if (! Key2PersonHashContains(link->getToKey())){
               qDebug() << "Link has To keyword that does not exist.";
               continue;
              }
          }
       }
    }
    return ;
}


void cData::AttachLinks(gPerson *){


}



void cData::save() const{
    QFile saveFile("save.json");
    if (!saveFile.open(QIODevice::WriteOnly)) {
           qWarning("Couldn't open save file.");
           return;
       }

    QJsonObject graphObject;
    write(graphObject);
    QJsonDocument saveDoc(graphObject);
    saveFile.write(saveDoc.toJson());


}
void cData::populatePersonTable(QTableWidget * table){
   table->setRowCount(graphicalPersons.size());
   table->setColumnCount(6);
   int row = 0;
   foreach (gPerson * person, graphicalPersons){
       QTableWidgetItem *item = new QTableWidgetItem(person->LastName());
       table->setItem(row,1,item);
       item = new QTableWidgetItem(person->FirstName());
       table->setItem(row,2,item);
       item = new QTableWidgetItem(person->Key());
       item->setTextColor(Qt::red);
       table->setItem(row,0,item);
       item = new QTableWidgetItem(QString::number(person->BirthYear()));
       table->setItem(row,3,item);
       qDebug() << ">>>"<< person->BirthYear();
       item = new QTableWidgetItem(QString::number(person->DeathYear()));
       table->setItem(row,4,item);
       item = new QTableWidgetItem(person->Profession1());
       table->setItem(row,5,item);


       row++;
   }
   table->setSortingEnabled(true);
}
void cData::populateLinkTable(QTableWidget * table){
   table->setRowCount(Links.size());
   table->setColumnCount(20);
   int row = 0;
   foreach (cLink * link, Links){
       QTableWidgetItem *item = new QTableWidgetItem(link->getFromKey());
       table->setItem(row,0,item);
       item = new QTableWidgetItem(link->getToKey());
       table->setItem(row,1,item);
       item = new QTableWidgetItem(link->NatureOfLink());
       table->setItem(row,2,item);
       item = new QTableWidgetItem(link->GetPositionOnFromPerson());
       table->setItem(row,3,item);
       item = new QTableWidgetItem(link->GetPositionOnToPerson());
       table->setItem(row,4,item);
       item = new QTableWidgetItem(link->TopOffset());
       table->setItem(row,5,item);
       item = new QTableWidgetItem(link->BottomOffset());
       table->setItem(row,6,item);
       row++;
   }
   table->setSortingEnabled(true);
}
void cData::MoveInvisibleToLimbo(){

    // move all gpersons marked invisible to Limbo container, and then update.
    QMutableListIterator<gPerson*> iter(graphicalPersons);
    while (iter.hasNext()){
        if (! iter.next()->Visible()){
           //Limbo.append(iter);
           iter.remove();
        }
    }

}
