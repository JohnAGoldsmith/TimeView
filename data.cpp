#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QGraphicsWidget>
#include <QGraphicsProxyWidget>
#include <QPushButton>
#include <QGraphicsDropShadowEffect>
#include <QStringList>
#include "data.h"
#include "clink.h"
#include "cscene.h"
#include "group.h"

cData::cData()
{
   topPosition = 1950;
}
void cData::Clear(){
    foreach (gPerson* person, graphicalPersons){
        delete person;
    }
    graphicalPersons.clear();
    foreach (cLink* link, Links){
        delete link;
    }
    Links.clear();
    foreach (cGroup* group, Groups){
        delete group;
    }
    Groups.clear();
    Limbo.clear();
    key2Person.clear();
    key2Link.clear();
    key2Group.clear();
    tempLines.clear();
}
cLink* cData::getLink(QString key){
    if (key2Link.contains(key)){
        return key2Link[key];
    }
    return NULL;
}
cLink* cData::CheckAndCreateNewLink(QString key){
    if (key2Link.contains(key)){
        return NULL;
    }
    cLink * link = new cLink(this);
    key2Link[key] = link;
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

void cData::A_analyzeLegacyCSVdata(){
    cLink * link;
    gPerson * person;
    QString outfileName, fromKey, toKey;
    cGroup * group;
    foreach (QString line1 , tempLines){
          QStringList line = line1.split(",");
          if (line.size() < 1)
           continue;
          if (line[0][0]== "!") {
            outfileName = line[1];
          } else if (line[0][0] == "#"){
            continue;
          } else if (line[0] == "P"){
            person = B_Legacy_CreateGraphicalPerson(line);
          } else if (line[0] == "L"){
            bool dummy;
            link = new cLink(dummy, line, this);
            Links.append(link);
          } else if (line[0] == "G"){              
            group = new cGroup(line);
            if (!validateNewGroup(group)){
               delete group;
               /*  send message of failure */
            }
          }
    }
    B_AddGPersonPtrsToLinks();
}


void cData::A_analyzeCSVdata(){
    gPerson * person;
    cLink * link;
    QString name;
    cGroup *  group;
    foreach (QString line1 , tempLines){
        QStringList line = line1.split(",");
        if (line.size() < 1)
            continue;
        if (line[0][0]== "!") {
            name = line[1];
        } else {
            if (line[0][0] == "#"){
                continue;
            } else if (line[0] == "P"){
                person = new gPerson(line);
                if (! validateNewPerson(person)){ //validate should include sending to collections
                    delete person;
                    /* send message of failure.  */
                }
            } else if (line[0] == "L"){
                link = new cLink(line, this);
                if (! validateNewLink(link)){ // validate should include sending to collections
                    delete link;
                    /* send message of failure. */
                }
            } else if (line[0] == "G"){
                group = new cGroup(line);
                if (!validateNewGroup(group)){
                    delete group;
                    /*  send message of failure */
                }
            }
        }
    }
    B_AddGPersonPtrsToLinks();
}
gPerson* cData::B_Legacy_CreateGraphicalPerson(QStringList line){
    QString key;
    float legacyTimeScale = 20.0;             // remove this from Scene.
    if (line.size() >= 8 && line[7].length() > 0){
        key = line[7];
    } else {
        key = line[2];
    }
    if ( Key2PersonHashContains(key) ) {
       qDebug() << "cData:  collision of graphicalPerson keys" << key << "No graphical unit created.";
       return NULL;
    };
    bool dummy = false;
    gPerson* person = new gPerson(dummy, line);
    key2Person[key]= person;
    qDebug() << "Data 160" << person->LastName();
    qDebug() << "Data 161" << graphicalPersons.size();
    graphicalPersons.append(person);
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
        fromgperson = Key2Person(fromKey);
        togperson = Key2Person(toKey);
        link->attachPersons(fromgperson,togperson);
    }
}


void cData::A_sendPersonsAndLinksToScene(cScene* scene){
    gPerson * fromPerson, * toPerson;
    foreach (cGroup * group, Groups){
       group->setY ( scene->ConvertYearToYcoor( group->Y() ) );
       scene->addItem(group);
       group->setPos(group->X(), group->Y());
       qDebug() << "data group line 200" << group->X() << group->Y();
    }
    foreach (gPerson * person, graphicalPersons){
        scene->addItem(person);
        person->Scene(scene); // why is this necessary?
        person->setPos( person->GetMemoryOfScreenPosition());
    }
    foreach (cLink * link, Links){
        if (! Key2PersonHashContains(link->getFromKey())){
            qDebug() << "From Key has no person " << link->getFromKey();
            continue;
        }
        if (! Key2PersonHashContains(link->getToKey())){
            qDebug() << "To Key has no person " << link->getToKey();
            continue;
        }
        fromPerson = Key2Person(link->getFromKey());
        toPerson = Key2Person(link->getToKey());
        scene->addItem(link);
        qDebug() << "data 210"<< link->getFromKey() << link->getToKey();
        link->setPos(fromPerson->pos());
    }

}
void cData::sendPersonsToColumnarScene(columnarScene * colscene){
    foreach (gPerson* person, graphicalPersons){
       colscene->addToColumnar(person);
    }
    colscene->sortColumnar();
    colscene->displayColumnar();
}
void cData::A_sendPersonsAndLinksToSceneJson(cScene* scene){
    gPerson * gPerson1, * gPerson2;

    foreach(cGroup * group, Groups){
        scene->addItem(group);
        group->setPos(group->MemoryOfPos());
    }

    foreach (gPerson * gperson, graphicalPersons){
        scene->addItem(gperson);
        gperson->Scene(scene); // why is this necessary? Why can't I get this from the gperson?
        gperson->setPos( gperson->GetMemoryOfScreenPosition() );
    }
    foreach (cLink * link, Links){
        //gPerson1 = link->GPersonFrom();
        //gPerson2 = link->GPersonTo();
        gPerson * fromPerson , * toPerson;
        fromPerson=Key2Person(link->getFromKey());
        if ( ! fromPerson   ){
            qDebug() << " DATA: Cannot send link to Scene   "<<link->getFromKey() << "missing " ;
            continue;
        }
        toPerson = Key2Person(link->getToKey());
        if ( ! toPerson ){
            qDebug() << "Cannot send link to Scene 124 "<<link->getToKey() << "Missing To gperson." ;
            continue;
        }
        //scene->AddLink(link);
        scene->addItem(link);
        link->setPos(fromPerson->pos());
    }
}

bool cData::validateNewPerson(gPerson * person){
    if (key2Person.contains(person->Key())){
        qDebug() << "data 240" << person->Key();
      /*  send message of collision */
            return false;
    }
    qDebug() <<"data 244 making new person" << person->Key();
     key2Person[person->Key()] = person;
     graphicalPersons.append(person);
     return true;
}
bool cData::validateNewLink(cLink* link){
    if (key2Link.contains(link->getKey())){
        /*  send message of collision   */
        return false;
    }
    key2Link[link->getKey()] = link;
    Links.append(link);
    return true;
}
bool cData::validateNewGroup(cGroup * group){
    qDebug() << group->Key();
    //int x = key2Group.size();
    //if (key2Group.contains(group->Key())){
    //  /*  send message of collision */
    //        return false;
   // }
    qDebug() << "data 247" << group->Key();
    key2Group[group->Key()] = group;
    Groups.append(group);
    return true;
}


void cData::write(QJsonObject &json) const{
    QJsonArray groupArray;
    foreach (const cGroup * group, Groups){
         QJsonObject groupObject;
         group->write(groupObject);
         groupArray.append(groupObject);
    }
    json["Groups"]=groupArray;


    QJsonArray linkArray;
    foreach (const cLink * link, Links){
         QJsonObject linkObject;
         link->write(linkObject);
         linkArray.append(linkObject);
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

    if (rootItem.contains("Groups") && rootItem["Groups"].isArray() ){
        QJsonArray groupArray = rootItem["Groups"].toArray();
        for (int index = 0; index < groupArray.size(); ++index){
            QJsonObject groupObject = groupArray[index].toObject();
            cGroup * group = new cGroup();
            group->read(groupObject);
            Groups.append(group);
        }
    }
    if (rootItem.contains("GraphicalPersons") && rootItem["GraphicalPersons"].isArray() ){
        QJsonArray gPersonArray = rootItem["GraphicalPersons"].toArray();
        for (int index = 0; index < gPersonArray.size(); ++index){
            QJsonObject gpObject = gPersonArray[index].toObject();
            QString key1 = gpObject["key"].toString();
            gPerson* gperson = new gPerson();
            gperson->read(gpObject);               /*  this ignores links   */
            key2Person[key1] = gperson;
            graphicalPersons.append(gperson);



            /*   This will be eliminated           */
            /* The next lines should go into Json-read function, *except* that they call a cData function which creates a new cLink */
            /*
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

                 if (! link->GPersonFrom()){
                    QString fromKey = link->getFromKey();
                    if (Key2PersonHashContains(fromKey)){
                        link->setPersonFrom(key2Person[fromKey]);
                    }
                 }
                 if (! link->GPersonTo() ) {
                    QString toKey = link->getToKey();
                    if (Key2PersonHashContains(toKey)){
                        link->setPersonTo(key2Person[toKey]);
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
            }*/
            /*  What is above here will be eliminated       */



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
               gPerson* gpersonFrom = key2Person[link->getFromKey()]; //remove
               gPerson* gpersonTo   = key2Person[link->getToKey()]; //remove
               // Add ptr to Link to FromPerson and to ToPerson
               // Add ptr to FromPerson and to ToPerson inside Link;

               // following line should be commented out:
               //link->attachPersons(gpersonFrom, gpersonTo);
               qDebug() << "data 373"<< gpersonFrom->LastName() << gpersonTo->LastName();
               gpersonFrom->AppendLink(link); //remove
               gpersonTo->AppendLink(link);//remove

               if (Links.contains(link)){
                   qDebug() << "Data: second attempt to enter link is ignored" << link->getKey();
               } else{
                   Links.append(link);
               }
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

    /*   Now we re-read the Json Persons, so that we can give them pointers to the Links  */
    /*   Hmm. This does not appear to be necessary at all.
    if (rootItem.contains("GraphicalPersons") && rootItem["GraphicalPersons"].isArray() ){
        QJsonArray gPersonArray = rootItem["GraphicalPersons"].toArray();
        for (int index = 0; index < gPersonArray.size(); ++index){
            QJsonObject gpObject = gPersonArray[index].toObject();
            QString key = gpObject["key"].toString();
            gPerson * person = Key2Person(key);
            if (gpObject.contains("Links") and gpObject["Links"].isArray()){
                QJsonArray linkArray = gpObject["Links"].toArray();
                for (int index = 0; index < linkArray.size(); index++){
                    QString linkkey = linkArray[index].toString();
                    cLink * link = Key2Link(linkkey);
                    person->AppendLink(link);
                }
            }
            if (gpObject.contains("TopLinks") and gpObject["TopLinks"].isArray()){
                QJsonArray toplinkArray = gpObject["TopLinks"].toArray();
                for (int index = 0; index < toplinkArray.size(); index++){
                    QString linkkey = toplinkArray[index].toString();
                    cLink * link = Key2Link(linkkey);
                    person->AppendTopLink(link);
                }
            }

        }
    }
    */

    return ;
}


void cData::AttachLinks(gPerson *){


}

QStringList cData::exportPersons2csv() const {
    QStringList output;
    foreach (gPerson* person, graphicalPersons){
        output.append(person->export2CSV());
    }
    return output;
}
QStringList cData::exportLinks2csv() const {
    QStringList linksList;
    foreach (cLink * link, Links){
        linksList << link->export2csv();
    }
    return linksList;
}
QStringList cData::exportGroups2csv() const{
    QStringList groupsList;
    foreach (cGroup * group, Groups){
        groupsList << group->export2csv();
    }
    return groupsList;
}
void cData::save(QString filename) const{
  QFile saveFile(filename + ".json");
  if (!saveFile.open(QIODevice::WriteOnly)) {
           qWarning("Couldn't open save file.");
           return;
    }
  QJsonObject graphObject;
  write(graphObject);
  QJsonDocument saveDoc(graphObject);
  saveFile.write(saveDoc.toJson());
  QFile saveFileCVS (filename + ".csv");
  if (!saveFileCVS.open(QIODevice::WriteOnly)) {
         qWarning("Couldn't open save file.");
         return;
   }
   QTextStream out(& saveFileCVS);
   QStringList csvList;
   csvList = exportPersons2csv();
   foreach (QString line, csvList){
       out << line << Qt::endl;
   }
   csvList.clear();
   csvList = exportLinks2csv();
   foreach (QString line, csvList){
       out << line << Qt::endl;
   }
   csvList.clear();
   csvList = exportGroups2csv();
   foreach (QString line, csvList){
       out << line << Qt::endl;
   }
   saveFileCVS.close();
}
void cData::populatePersonTable(QTableWidget * table){
   table->setRowCount(graphicalPersons.size());
   table->setColumnCount(9);
   int row = 0;
   foreach (gPerson * person, graphicalPersons){
       QTableWidgetItem *item;
       item = new QTableWidgetItem(person->Key());
       table->setItem(row,0,item);
       item = new QTableWidgetItem(person->LastName());
       table->setItem(row,1,item);
       item = new QTableWidgetItem(person->FirstName());
       table->setItem(row,2,item);
       item = new QTableWidgetItem(QString::number(person->BirthYear()));
       table->setItem(row,3,item);
       item = new QTableWidgetItem(QString::number(person->DeathYear()));
       table->setItem(row,4,item);
       item = new QTableWidgetItem(person->Profession1());
       table->setItem(row,5,item);
       item = new QTableWidgetItem(QString::number(person->GetMemoryOfScreenPosition().x()));
       table->setItem(row,6,item);
       item = new QTableWidgetItem(QString::number(person->GetMemoryOfScreenPosition().y()));
       table->setItem(row,7,item);
       item = new QTableWidgetItem(person->Profession1());
       table->setItem(row,8,item);


       row++;
   }
   table->setSortingEnabled(true);
}
void cData::populateLinkTable(QTableWidget * table){
   table->setRowCount(Links.size());
   table->setColumnCount(20);
   int row = 0;
   QTableWidgetItem *headerItem = new QTableWidgetItem("From key");
   table->setHorizontalHeaderItem(0, headerItem);
   headerItem = new QTableWidgetItem("To key");
   table->setHorizontalHeaderItem(1, headerItem);
   headerItem = new QTableWidgetItem("Nature of link");
   table->setHorizontalHeaderItem(2, headerItem);
   headerItem = new QTableWidgetItem("From Pos");
   table->setHorizontalHeaderItem(3, headerItem);
   headerItem = new QTableWidgetItem("To Pos");
   table->setHorizontalHeaderItem(4, headerItem);
   headerItem = new QTableWidgetItem("Top offset");
   table->setHorizontalHeaderItem(5, headerItem);
   headerItem = new QTableWidgetItem("Bottom offset");
   table->setHorizontalHeaderItem(6, headerItem);
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
       item = new QTableWidgetItem(QString::number(int(link->TopOffset())) );
       table->setItem(row,5,item);
       item = new QTableWidgetItem(QString::number(int(link->BottomOffset())));
       table->setItem(row,6,item);
       row++;
   }
   table->setSortingEnabled(true);
}

void cData::GrayedPersons2Invisible(){
    // move all gpersons marked invisible to Limbo container, and then update.
    foreach(gPerson* person, graphicalPersons){
        if (person->Grayed()){
            person->setGrayed(false);
            person->setVisible(false);
            person->update();
        }
    }
}
void cData::InvisiblePersons2Grayed(){
    foreach(gPerson* person, graphicalPersons){
        if (! person->isVisible()){
            person->setGrayed(true);
            person->setVisible(true);
            person->update();
        }
    }
}
