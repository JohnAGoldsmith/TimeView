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
    void write2csv();

    QList<cLink*>*    getLinks(){return &Links;};
    cLink* getLink(QString);
    cLink* CheckAndCreateNewLink(QString);  // only creates a new link if the key is not already in Links;

    bool validateNewPerson(gPerson*); // checks for not duplicate, and if OK adds to collection.
    bool validateNewLink(cLink*);

    void A_ReadCSV(QString filename);
    void A_ReadJson(QString filename);
    void A_analyzeLegacyCSVdata();
    void A_analyzeCSVdata();
    QStringList exportPersons2csv() const ;
    QStringList exportLinks2csv() const;
    void A_sendPersonsAndLinksToScene(cScene *);
    void A_sendPersonsAndLinksToSceneJson(cScene *);
    gPerson* Key2Person(QString key){return key2Person[key];} // was getpersonfromkey
    bool  Key2PersonHashContains (QString key ) {return  key2Person.contains(key);}
    cLink* Key2Link(QString key) {return key2Link[key];}
    void save() const;

    //void addDataPerson(dPerson*);
    int TopPosition() {return topPosition;}
    //dPerson* CreateDataPerson(QStringList line);
    gPerson* B_CreateGraphicalPerson(QStringList line);
    void B_AddGPersonPtrsToLinks();

    void Clear();
    void AttachLinks(gPerson * );

    void populatePersonTable(QTableWidget*);
    void populateLinkTable(QTableWidget*);

    void GrayedPersons2Invisible();
    void InvisiblePersons2Grayed();
    //void MoveInvisiblePersonsToLimbo();
    //void MoveLimboPersonsToInvisible();
private:
    QList<dPerson*> dataPersons;
    QList<gPerson*> graphicalPersons;
    QList<cLink*>   Links;
    QList<gPerson*> Limbo;
    QHash<QString, gPerson*>  key2Person;
    QHash<QString, cLink*> key2Link;
    QStringList     tempLines;
    //int             timeScale;
    int             topPosition;
};

#endif // DATA_H
