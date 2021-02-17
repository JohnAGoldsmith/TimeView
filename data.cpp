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
   //timeScale = 5;
   topPosition = 1950;
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
void cData::analyzeData(){
    dPerson* dPerson1;
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
                   dPerson1 = new dPerson(line);
                   dataPersons.append(dPerson1);
                   Key2dataPerson[dPerson1->Key()] = dPerson1;
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
void cData::sendPersonsAndLinksToScene(cScene* scene){
    QGraphicsRectItem* rItem;
    QGraphicsLineItem * line;
    gPerson * gPerson1, * gPerson2;
    QString sPerson1, sPerson2;
    dPerson* dPerson1, * dPerson2;
    cLink * link;
    foreach (dPerson * dperson, dataPersons){
        scene->AddPerson(dperson);
    }
    foreach (cLink * link, Links){
        if ( Key2dataPersonHashContains(link->getFromKey())) {
                    dPerson1 =  getPersonFromKey(link->getFromKey());
        }else{
            qDebug() << "missing someone" << link->getFromKey() ;
            continue;
        }
        if ( Key2dataPersonHashContains(link->getToKey())) {
               dPerson2 =  getPersonFromKey(link->getToKey());
        }else{
            qDebug() << "missing" ;
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

    json["Persons"] = personArray;
    return ;
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
