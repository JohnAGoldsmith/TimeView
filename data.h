#ifndef DATA_H
#define DATA_H
#include <QHash>
#include "gperson.h"
#include "dataperson.h"
#include "clink.h"

class cData
{
public:
    cData();
    QList<dPerson*>* getPersons(){return &dataPersons;};
    QList<cLink*>*    getLinks(){return &Links;};

    void ReadCSV();
    void analyzeData();
    void sendPersonsToScene(cScene *);
    dPerson* getPersonFromKey(QString key){return Key2dataPerson[key];}
    bool  Key2dataPersonHashContains (QString key ) {return  Key2dataPerson.contains(key);}
private:
    QList<dPerson*> dataPersons;
    QList<cLink*>   Links;
    QHash<QString, dPerson*>  Key2dataPerson;
    QStringList     tempLines;
    //int             timeScale;
    int             topPosition;
};

#endif // DATA_H
