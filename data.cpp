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
void cData::RemoveLink(cLink* link){
    Links.removeAll(link);
    key2Link.remove(link->getKey());
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
            //person->SortLinksOnEachSide();
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
    B_AddPtrsToPersonsAndLinks();
    AttachLinkKeysToPersonSides();
    //AppendLinkAndAddLinkKeyToSide(link);
    //SortLinksOnEachSide();
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
    B_AddPtrsToPersonsAndLinks();
    AttachLinkKeysToPersonSides();
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
    graphicalPersons.append(person);
    return person;
}

void cData::B_AddPtrsToPersonsAndLinks(){
    QString fromKey, toKey;
    gPerson * fromPerson, *toPerson;
    QList<cLink*> linksToDelete;

    foreach (cLink* link, Links){
        fromKey = link->getFromKey();
        toKey = link->getToKey();
        if (! Key2PersonHashContains(fromKey)){
            qDebug() << "Link containing "<<fromKey << "cannot be made;"<<fromKey << "is missing.";
            linksToDelete.append(link);
            continue;
        }
        if (! Key2PersonHashContains(toKey)){
            qDebug() << "Link containing "<<toKey << "cannot be made;"<<toKey << "is missing.";
            linksToDelete.append(link);
            continue;
        }
        fromPerson = Key2Person(fromKey);
        toPerson = Key2Person(toKey);
        link->setPersonFrom(fromPerson);
        link->setPersonTo(toPerson);
        fromPerson->AppendLink(link);
        toPerson->AppendLink(link);
    }
    foreach (cLink* link, linksToDelete){
        RemoveLink(link);
    }


}
void cData::AttachLinkKeysToPersonSides(){
    foreach (cLink* link, Links){
        QString FromPosition = link->GetPositionOnFromPerson();
        QString ToPosition   = link->GetPositionOnToPerson();
        QString key = link->getKey();

        if(FromPosition =="Top"){
            link->FromPerson()->AppendTopLinkKey(key);}
        else if(FromPosition =="Right"){
            link->FromPerson()->AppendRightLinkKey(key);}
        else if(FromPosition =="Bottom"){
            link->FromPerson()->AppendBottomLinkKey(key);}
        else if(FromPosition=="Left"){
            link->FromPerson()->AppendLeftLinkKey(key);}

        if(ToPosition =="Top"){
            link->ToPerson()->AppendTopLinkKey(key);}
        else if(ToPosition =="Right"){
            link->ToPerson()->AppendRightLinkKey(key);}
        else if(ToPosition =="Bottom"){
            link->ToPerson()->AppendBottomLinkKey(key);}
        else if(ToPosition =="Left"){
            link->ToPerson()->AppendLeftLinkKey(key);}
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
        };
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
     key2Person[person->Key()] = person;
     graphicalPersons.append(person);
     return true;
}
bool cData::validateNewLink(cLink* link){
    if (key2Link.contains(link->getKey())){
        /*  send message of collision   */
        return false;
    }
    if (! Key2PersonHashContains(link->getFromKey()) ){
        qDebug() << "We cannot include this link whose From person is not present"<< link->getFromKey();
        return false;
    }
    if (! Key2PersonHashContains(link->getToKey()) ){
        qDebug() << "We cannot include this link whose To person is not present"<< link->getToKey();
        return false;
    }
    key2Link[link->getKey()] = link;
    Links.append(link);
    return true;
}
bool cData::validateNewGroup(cGroup * group){
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
            gPerson* person = new gPerson();
            person->read(gpObject);
            key2Person[person->Key()] = person;
            graphicalPersons.append(person);

            auto effect = new QGraphicsDropShadowEffect();
            effect->setBlurRadius(5);
            effect->setXOffset(15);
            effect->setYOffset(5);
            effect->setColor(QColor("#555555"));
            person->setGraphicsEffect(effect);
            }
    }

    if (  rootItem.contains ("Links") && rootItem["Links"].isArray()  ){
       QJsonArray linkArray = rootItem["Links"].toArray();
       for (int index = 0; index < linkArray.size(); ++index){
           QJsonObject linkObject = linkArray[index].toObject();
           cLink * link = CheckAndCreateNewLink(linkObject["Key"].toString());
           if (link){
               link->read(linkObject);
          }
       }
    }

    foreach (gPerson * person, graphicalPersons ){
        foreach(QString linkKey, *person->GetTopLinkKeys()){
            cLink* link = Key2Link(linkKey);
            person->SetKey2Link(linkKey, link);
            person->AppendLinkAndAddLinkKeyToSide(link);
        }
        foreach(QString linkKey, *person->GetRightLinkKeys()){
            cLink* link = Key2Link(linkKey);
            person->SetKey2Link(linkKey, link);
            person->AppendLinkAndAddLinkKeyToSide(link);
        }
        foreach(QString linkKey, *person->GetBottomLinkKeys()){
            cLink* link = Key2Link(linkKey);
            person->SetKey2Link(linkKey, link);
            person->AppendLinkAndAddLinkKeyToSide(link);
        }
        foreach(QString linkKey, *person->GetLeftLinkKeys()){
            cLink* link = Key2Link(linkKey);
            person->SetKey2Link(linkKey, link);
            person->AppendLinkAndAddLinkKeyToSide(link);
        }
    }

    return;
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
