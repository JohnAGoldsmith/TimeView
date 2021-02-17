#ifndef DATA_H
#define DATA_H
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QHash>
#include "gperson.h"
#include "dataperson.h"
#include "clink.h"

// There should be an enum for DataType (Person, Link) but enum in Qt is complicated.

class cData
{
public:
    cData();
    QString thisDataType;

    void read(const QJsonObject & json);
    void write(QJsonObject &json) const;

    QList<dPerson*>* getPersons(){return &dataPersons;};
    QList<cLink*>*    getLinks(){return &Links;};

    void ReadCSV();
    void analyzeData();
    void sendPersonsAndLinksToScene(cScene *);
    dPerson* getPersonFromKey(QString key){return Key2dataPerson[key];}
    bool  Key2dataPersonHashContains (QString key ) {return  Key2dataPerson.contains(key);}
    void save() const;
private:
    QList<dPerson*> dataPersons;
    QList<cLink*>   Links;
    QHash<QString, dPerson*>  Key2dataPerson;
    QStringList     tempLines;
    //int             timeScale;
    int             topPosition;
};

#endif // DATA_H
