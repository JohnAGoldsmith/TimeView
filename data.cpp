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
    gPerson * gperson;
    cLink * l;
    QString outfileName;
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
    gPerson* gPerson1 = new gPerson(dperson);
    dperson->setGraphicPerson(gPerson1);
    qDebug() << 85 << graphicalPersons.size();
    graphicalPersons.append(gPerson1);
    qDebug() << 86 << gPerson1->LastName() << graphicalPersons.size();

}

void cData::sendPersonsAndLinksToScene(cScene* scene){
    QGraphicsRectItem* rItem;
    QGraphicsLineItem * line;
    gPerson * gPerson1, * gPerson2;
    QString sPerson1, sPerson2;
    dPerson* dPerson1, * dPerson2, *dperson;
    cLink * link;

    foreach (gPerson * gperson, graphicalPersons){

        scene->addItem(gperson);
        //qDebug() << 99 << gperson->LastName() << graphicalPersons.size();
        dperson = gperson->getDPerson();
        QPointF transformedCoordinates (dperson->Xpos() * scene->ScaleFactor(), dperson->Ypos() * scene->TimeScale());
        gperson->setPos(transformedCoordinates);
    }
    foreach (cLink * link, Links){
        if ( Key2dataPersonHashContains(link->getFromKey())) {
                    dPerson1 =  getDataPersonFromKey(link->getFromKey());
        }else{
            //qDebug() << "cData: missing someone" << link->getFromKey() ;
            continue;
        }
        if ( Key2dataPersonHashContains(link->getToKey())) {
               dPerson2 =  getDataPersonFromKey(link->getToKey());
        }else{
            qDebug() << "cData: missing person for link;" << link->getToKey(); ;
            continue;
        }

        gPerson1 = dPerson1->get_gPerson();
        gPerson2 = dPerson2->get_gPerson();

        link->attachGraphicalPersons(gPerson1, gPerson2);
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
         linkArray.append(gPersonObject);
    }
    json["GraphicalPersons"]=gPersonArray;
    return ;
}
void cData::ReadJson() {
    QFile file ("./save.json");
    qDebug() << QDir::currentPath();
    if (! file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QByteArray thisData = file.readAll();
    QJsonDocument loadDoc(QJsonDocument::fromJson(thisData));

    //read(loadDoc.object());


    QJsonObject rootItem = loadDoc.object(); // this should be the root item;


    if (  rootItem.contains ("DataPersons") && rootItem["DataPersons"].isArray()  ){
       QJsonArray dpArray = rootItem["DataPersons"].toArray();
       qDebug() << 155 << dpArray.size();

       for (int index = 0; index < dpArray.size(); ++index){
           QJsonObject dpObject = dpArray[index].toObject();
           dPerson * dperson = new dPerson();
           dperson->read(dpObject);
           qDebug() << dperson->display();
           dataPersons.append(dperson);
           if (Key2dataPerson.contains(dperson->Key())){
                 qWarning ("Collision of Keys for dataPersons; new person created nonetheless.");
           }
           Key2dataPerson[dperson->Key()] = dperson;
       }
    }
    if (  rootItem.contains ("DataPersons") && rootItem["DataPersons"].isArray()  ){
       QJsonArray dpArray = rootItem["DataPersons"].toArray();
       qDebug() << 155 << dpArray.size();

       for (int index = 0; index < dpArray.size(); ++index){
           QJsonObject dpObject = dpArray[index].toObject();
           dPerson * dperson = new dPerson();
           dperson->read(dpObject);
           qDebug() << dperson->display();
           dataPersons.append(dperson);
           if (Key2dataPerson.contains(dperson->Key())){
                 qWarning ("Collision of Keys for dataPersons; new person created nonetheless.");
           }
           Key2dataPerson[dperson->Key()] = dperson;
       }
    }


    return ;
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
