#ifndef CLINK_H
#define CLINK_H
#include <QStringList>

class cPath;

class cLink
{

    public:
        cLink();
        cLink(QStringList);
    private:
        QString fromPersonKey;
        QString toPersonKey;
        cPath  * path;
        QString natureOfLink;

};

#endif // CLINK_H
