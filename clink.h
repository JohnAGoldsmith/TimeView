#ifndef CLINK_H
#define CLINK_H
#include <QStringList>

class cPath;
class gPerson;
class cScene;

class cLink
{

    public:
        cLink();
        cLink(QStringList);
        QString getFromKey(){return fromPersonKey;}
        QString getToKey(){return toPersonKey;}
        void attachScene(cScene*);
        void attachGraphicalPersons(gPerson *, gPerson *);
        gPerson* GPersonFrom(){return gPersonFrom;}
        gPerson* GPersonTo() {return gPersonTo;}
    private:
        QString fromPersonKey;
        QString toPersonKey;
        cPath  * path;
        QString natureOfLink;
        gPerson* gPersonFrom;
        gPerson* gPersonTo;

};

#endif // CLINK_H
