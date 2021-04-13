#ifndef DATA_H
#define DATA_H
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QHash>
#include <QTableWidget>
#include "gperson.h"
#include "clink.h"
#include "columnarscene.h"
#include "group.h"

//class cGroup;

// There should be an enum for DataType (Person, Link) but enum in Qt is complicated.

class cData
{
public:
    cData();

    QString thisDataType;

    void read(const QJsonObject & json) ;
    void write(QJsonObject &json) const;


    QList<cLink*>*    getLinks(){return &Links;};
    cLink* getLink(QString);
    cLink* CheckAndCreateNewLink(QString);  // only creates a new link if the key is not already in Links;
    void RemoveLink (cLink* link);

    bool validateNewPerson(gPerson*); // checks for not duplicate, and if OK adds to collection.
    bool validateNewLink(cLink*);
    bool validateNewGroup (cGroup*);

    void A_ReadCSV(QString filename);
    void A_ReadJson(QString filename);
    void A_analyzeLegacyCSVdata();
    void A_analyzeCSVdata();

    QStringList exportPersons2csv() const ;
    QStringList exportLinks2csv() const;
    QStringList exportGroups2csv() const;

    void A_sendPersonsAndLinksToScene(cScene *);
    void A_sendPersonsAndLinksToSceneJson(cScene *);
    void sendPersonsToColumnarScene(columnarScene * );
    gPerson* Key2Person(QString key){return key2Person[key];} // was getpersonfromkey
    cGroup* Key2Group(QString key) {return key2Group[key];}
    bool  Key2PersonHashContains (QString key ) {return  key2Person.contains(key);}
    cLink* Key2Link(QString key) {return key2Link[key];}
    void save(QString filename) const;



    int TopPosition() {return topPosition;}

    gPerson* B_Legacy_CreateGraphicalPerson(QStringList line);
    void B_AddPtrsToPersonsAndLinks();

    void Clear();
    void AttachLinks(gPerson * );

    void populatePersonTable(QTableWidget*);
    void populateLinkTable(QTableWidget*);

    void GrayedPersons2Invisible();
    void InvisiblePersons2Grayed();
    //void MoveInvisiblePersonsToLimbo();
    //void MoveLimboPersonsToInvisible();
    void AttachLinkKeysToPersonSides();
private:

    QList<gPerson*> graphicalPersons;
    QList<cLink*>   Links;
    QList<cGroup*>  Groups;
    QList<gPerson*> Limbo;
    QHash<QString, gPerson*>key2Person;
    QHash<QString, cLink*>  key2Link;
    QHash<QString, cGroup* >key2Group;
    QStringList     ErrorLog;
    QStringList     tempLines;
    int             topPosition;

};

#endif // DATA_H
