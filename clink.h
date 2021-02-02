#ifndef CLINK_H
#define CLINK_H
#include <QStringList>

class cPath;

class cLink
{

    public:
        cLink();
        cLink(QStringList);
        QString getFromKey(){return fromPersonKey;}
        QString getToKey(){return toPersonKey;}
    private:
        QString fromPersonKey;
        QString toPersonKey;
        cPath  * path;
        QString natureOfLink;

};

#endif // CLINK_H
