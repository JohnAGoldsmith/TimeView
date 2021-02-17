#ifndef CLINK_H
#define CLINK_H
#include <QStringList>
#include <QRectF>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QWidget>
#include <QGraphicsItem>
//#include "mainwindow.h"

class cPath;
class gPerson;
class cScene;

// There should be an ENUM for LinkPosition, but enum in Qt is complicated.

class cLink: public QGraphicsItem
{

    public:
        cLink();
        cLink(QStringList & );
        ~cLink();
        QRectF boundingRect() const;
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

        void write(QJsonObject & json) const;

        QString getFromKey(){return fromPersonKey;}
        QString getToKey(){return toPersonKey;}
        //void attachScene(cScene*);
        void attachGraphicalPersons(gPerson *, gPerson *);
        gPerson* GPersonFrom()const {return gPersonFrom;}
        gPerson* GPersonTo() const {return gPersonTo;}
        void Offset(float f) {offset = f;}
        QString GetPositionOnFromPerson(){return PositionOnFromPerson;}
    private:
        QString fromPersonKey;
        QString toPersonKey;
       // cPath  * path;
        QString natureOfLink;
        gPerson* gPersonFrom;
        gPerson* gPersonTo;
        QString PositionOnFromPerson;
        QString PositionOnToPerson;
        float offset;

};

#endif // CLINK_H
