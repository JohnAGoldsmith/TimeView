#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QDir>
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
    QString separator(",");    int topPosition = 1950;
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
                   qDebug() << "analyzing Data line 50 linking data person to hash." << dPerson1->Key() ;
                   qDebug() << "analyzing Data line 51"<< getPersonFromKey(dPerson1->Key()) ->LastName();
               } else {
                   if (line[0] == "L"){
                       l = new cLink(line);
                       Links.append(l);
                       qDebug() << "analyzing Data New link";
                   }
               }
           }
       }
    }
}
void cData::sendPersonsToScene(cScene* scene){
    QGraphicsRectItem* rItem;
    QGraphicsLineItem * line;
    gPerson * gPerson1, * gPerson2;
    QString sPerson1, sPerson2;
    dPerson* dPerson1, * dPerson2;
    cLink * link;
    foreach (dPerson * dperson, dataPersons){
         gPerson1 = new gPerson(scene, dperson);
         dperson->set_gPerson(gPerson1);
    }
    foreach (cLink * link, Links){
        if ( Key2dataPersonHashContains(link->getFromKey())) {
                    dPerson1 =  getPersonFromKey(link->getFromKey());
        }else{
            qDebug() << "missing someone" ;
            continue;
        }
        if ( Key2dataPersonHashContains(link->getToKey())) {
               dPerson2 =  getPersonFromKey(link->getToKey());
        }else{
            qDebug() << "missing" ;
            continue;
        }
        qDebug() << dPerson1->LastName();
        gPerson1 = dPerson1->get_gPerson();
        qDebug() << dPerson2->LastName();
        gPerson2 = dPerson2->get_gPerson();

        qDebug() << "a" << gPerson1;
        qDebug() << "b" << gPerson1->LastName() << gPerson2->LastName();
        line = new QGraphicsLineItem(gPerson1->x(),gPerson1->y(),gPerson2->x(),gPerson2->y());
    }
}
