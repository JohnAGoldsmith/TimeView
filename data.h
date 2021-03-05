#ifndef DATA_H
#define DATA_H
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QHash>
#include <QTableWidget>
#include "gperson.h"
#include "dataperson.h"
#include "clink.h"

// There should be an enum for DataType (Person, Link) but enum in Qt is complicated.

class cData
{
public:
    cData();
    QString thisDataType;

    void read(const QJsonObject & json) ;
    void write(QJsonObject &json) const;

    QList<dPerson*>* getDataPersons(){return &dataPersons;};
    QList<cLink*>*    getLinks(){return &Links;};

    void A_ReadCSV(QString filename);
    void A_ReadJson(QString filename);
    void A_analyzeData();
    void A_sendPersonsAndLinksToScene(cScene *);
    void A_sendPersonsAndLinksToSceneJson(cScene *);
    //dPerson* getDataPersonFromKey(QString key){return Key2dataPerson[key];}
    gPerson* getGraphicPersonFromKey(QString key){return Key2graphicalPerson[key];}
    bool  Key2dataPersonHashContains (QString key ) {return  Key2dataPerson.contains(key);}
    bool  Key2graphicalPersonHashContains (QString key ) {return  Key2graphicalPerson.contains(key);}
    void save() const;

    void addDataPerson(dPerson*);
    int TopPosition() {return topPosition;}
    dPerson* CreateDataPerson(QStringList line);
    gPerson* B_CreateGraphicalPerson(QStringList line);
    void B_AddGPersonPtrsToLinks();

    void Clear();
    void AttachLinks(gPerson * );

    void populatePersonTable(QTableWidget*);
    void populateLinkTable(QTableWidget*);

    void MoveInvisibleToLimbo();
private:
    QList<dPerson*> dataPersons;
    QList<gPerson*> graphicalPersons;
    QList<cLink*>   Links;
    QList<gPerson*> Limbo;
    QHash<QString, dPerson*>  Key2dataPerson;
    QHash<QString, gPerson*>  Key2graphicalPerson;
    QStringList     tempLines;
    //int             timeScale;
    int             topPosition;
};

#endif // DATA_H
