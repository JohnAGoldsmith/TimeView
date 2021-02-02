#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QDir>
#include "data.h"

cData::cData()
{
   timeScale = 5;
   topPosition = 1950;
}

void cData::ReadCSV()
{
    // Open csv-file
    QFile file("./test.csv");
    qDebug() << QDir::currentPath();
    if (! file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    // Read data from file
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
    //QList<cdataPerson*> Persons;
    //QList<cLink*> Links;
    cdataPerson* p;
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
                   p = new cdataPerson(line);
                   Persons.append(p);
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
void cData::sendPersonsToScene(cScene* scene){
    QGraphicsRectItem* rItem;
    cPerson * graphicPerson;

    foreach (cdataPerson * person, Persons){
         graphicPerson = new cPerson(scene, person->firstName, person->lastName, person->xpos * 50, (topPosition - person->birthYear) * timeScale);
         qDebug() << 71 << person->birthYear;
    }
}
