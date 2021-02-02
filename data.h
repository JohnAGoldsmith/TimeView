#ifndef DATA_H
#define DATA_H

#include "cperson.h"
#include "dataperson.h"
#include "clink.h"

class cData
{
public:
    cData();
    QList<cdataPerson*>* getPersons(){return &Persons;};
    QList<cLink*>*    getLinks(){return &Links;};

    void ReadCSV();
    void analyzeData();
    void sendPersonsToScene(cScene *);

private:
    QList<cdataPerson*> Persons;
    QList<cLink*>   Links;
    QStringList     tempLines;
    int             timeScale;
    int             topPosition;
};

#endif // DATA_H
