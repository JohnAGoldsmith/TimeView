#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QDir>
#include <QFile>
#include "data.h"
#include "clink.h"
#include "cscene.h"

cData::cData()
{
   topPosition = 1950;
}


void cData::analyzeData(){
    dPerson* dperson;
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
                   dperson = CreateDataPerson(line);
                   gperson = CreateGraphicalPerson(dperson);
               } else {
                   if (line[0] == "L"){
                        l = new cLink(line);
                        Links.append(l);
                   }
               }
           }
       }
    }
    AddGPersonPtrsToLinks();
}

dPerson* cData::CreateDataPerson(QStringList line){
    dPerson * dPerson1 = new dPerson(line);
    dataPersons.append(dPerson1);
    if (Key2dataPerson.contains(dPerson1->Key())){
                    qWarning ("Collision of Keys for dataPersons; new person created nonetheless.");
    }
    Key2dataPerson[dPerson1->Key()] = dPerson1;
    return dPerson1;
}

gPerson* cData::CreateGraphicalPerson(dPerson * dperson){
    if (Key2graphicalPersonHashContains(dperson->Key())) {
       qDebug() << "cData:  collision of graphicalPerson keys" << dperson->LastName()<< "No graphical unit created.";
       return NULL;
    }
    dperson->Ypos ( TopPosition() - dperson->BirthYear() ) ;
    gPerson* gperson = new gPerson(dperson);
    Key2graphicalPerson[dperson->Key()]= gperson;
    dperson->setGraphicPerson(gperson);
    graphicalPersons.append(gperson);
}

void cData::AddGPersonPtrsToLinks(){
    QString fromKey, toKey;
    gPerson * fromgperson, *togperson;
    foreach (cLink* link, Links){
        fromKey = link->getFromKey();
        toKey = link->getToKey();
        if (! Key2graphicalPersonHashContains(fromKey)){
            qDebug() << "Link containing "<<fromKey << "and" << toKey << "cannot be made;"<<fromKey << "is missing.";
            continue;
        }
        if (! Key2graphicalPersonHashContains(toKey)){
            qDebug() << "Link containing "<<fromKey << "and" << toKey << "cannot be made;"<<toKey << "is missing.";
            continue;
        }
        //qDebug() << "data line 83"<<fromKey;
        //qDebug() << "data line 84"<< toKey;
        fromgperson = getGraphicPersonFromKey(fromKey);
        //qDebug() << "data line 86"<< fromgperson->LastName();
        togperson = getGraphicPersonFromKey(toKey);
        link->attachGraphicalPersons(fromgperson,togperson);
    }
}


void cData::sendPersonsAndLinksToScene(cScene* scene){
    gPerson * gPerson1, * gPerson2;
    dPerson* dPerson1, * dPerson2, *dperson;

    foreach (gPerson * gperson, graphicalPersons){
        scene->addItem(gperson);
        dperson = gperson->getDPerson();
        QPointF transformedCoordinates (dperson->Xpos() * scene->ScaleFactor(), dperson->Ypos() * scene->TimeScale());
        gperson->setPos(transformedCoordinates);
        gperson->rememberPos(transformedCoordinates);
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
void cData::sendPersonsAndLinksToSceneJson(cScene* scene){
    gPerson * gPerson1, * gPerson2;
    foreach (gPerson * gperson, graphicalPersons){
        scene->addItem(gperson);
        gperson->setPos(gperson->Xpos(), gperson->Ypos());
        if (gperson->LastName() == "Bourdieu" )
            qDebug() << "cdata sending Bourdieu to screen xpos"<< gperson->Xpos();
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
    QJsonArray personArray;
    foreach (const  dPerson *  dperson, dataPersons){
      QJsonObject personObject;
      dperson->write(personObject);
      personArray.append(personObject);
    }
    json["DataPersons"] = personArray;

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
void cData::ReadJson() {
    QFile file ("./save.json");
    if (! file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QByteArray thisData = file.readAll();
    QJsonDocument loadDoc(QJsonDocument::fromJson(thisData));
    QJsonObject rootItem = loadDoc.object(); // this should be the root item;

    if (  rootItem.contains ("DataPersons") && rootItem["DataPersons"].isArray()  ){
       QJsonArray dpArray = rootItem["DataPersons"].toArray();
       for (int index = 0; index < dpArray.size(); ++index){
           QJsonObject dpObject = dpArray[index].toObject();
           dPerson * dperson = new dPerson();  // create a new constructor for Json!!
           dperson->read(dpObject);
           dataPersons.append(dperson);
           if (Key2dataPerson.contains(dperson->Key())){
                 qWarning ("Collision of Keys for dataPersons; new person created nonetheless.");
           }
           Key2dataPerson[dperson->Key()] = dperson;
       }
    }
    if (rootItem.contains("GraphicalPersons") && rootItem["GraphicalPersons"].isArray() ){
        QJsonArray gPersonArray = rootItem["GraphicalPersons"].toArray();
        for (int index = 0; index < gPersonArray.size(); ++index){
            QJsonObject gpObject = gPersonArray[index].toObject();
            QString key1 = gpObject["key"].toString();
            if (!Key2dataPersonHashContains (key1)){
                qDebug() << "!!! Trying to make a gperson with illicit key" << key1;
                continue;
            }
            dPerson* dperson = Key2dataPerson[key1];
            gPerson* gperson = new gPerson(dperson);
            gperson->read(gpObject);
            Key2graphicalPerson[key1] = gperson;
            graphicalPersons.append(gperson);
            }
    }

    if (  rootItem.contains ("Links") && rootItem["Links"].isArray()  ){
       QJsonArray linkArray = rootItem["Links"].toArray();
       for (int index = 0; index < linkArray.size(); ++index){
           QJsonObject linkObject = linkArray[index].toObject();
           cLink * link = new cLink();
           link->read(linkObject);

           if (! Key2graphicalPersonHashContains(link->getFromKey())){
               qDebug() << "Link has From keyword that does not exist.";
               continue;
           }
           if (! Key2graphicalPersonHashContains(link->getToKey())){
               qDebug() << "Link has To keyword that does not exist.";
               continue;
           }

           gPerson* gpersonFrom = Key2graphicalPerson[link->getFromKey()];
           gPerson* gpersonTo   = Key2graphicalPerson[link->getToKey()];
           link->attachGraphicalPersons(gpersonFrom, gpersonTo);
           Links.append(link);

       }
    }

    return ;
}

/*
void cLink::attachGraphicalPersons(gPerson * person1, gPerson * person2){
 gPersonFrom = person1;
 gPersonTo = person2;
 person1->AppendLink(this);
 person2->AppendLink(this);
}
*/
void cData::AttachLinks(gPerson *){


}
void cData::ReadCSV()
{

    QFile file("./test.csv");
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
   table->setRowCount(dataPersons.size());
   table->setColumnCount(20);
   int row = 0;
   foreach (dPerson * dperson, dataPersons){
       QTableWidgetItem *item = new QTableWidgetItem(dperson->LastName());
       table->setItem(row,0,item);
       item = new QTableWidgetItem(dperson->FirstName());
       table->setItem(row,1,item);
       item = new QTableWidgetItem(dperson->Key());
       table->setItem(row,2,item);
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
       row++;
   }
   table->setSortingEnabled(true);
}
