#ifndef CLINK_H
#define CLINK_H
#include <QStringList>
#include <QRectF>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QWidget>
#include <QGraphicsItem>

class cPath;
class gPerson;
class cScene;

enum LinkPosition {Top, Left, Right, Bottom};

class cLink: public QGraphicsItem
{

    public:
        cLink();
        cLink(QStringList & );
        ~cLink();
        QRectF boundingRect() const;
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
        QString getFromKey(){return fromPersonKey;}
        QString getToKey(){return toPersonKey;}
        //void attachScene(cScene*);
        void attachGraphicalPersons(gPerson *, gPerson *);
        gPerson* GPersonFrom()const {return gPersonFrom;}
        gPerson* GPersonTo() const {return gPersonTo;}
        void Offset(float f) {offset = f;}
        LinkPosition GetPositionOnFromPerson(){return PositionOnFromPerson;}
    private:
        QString fromPersonKey;
        QString toPersonKey;
       // cPath  * path;
        QString natureOfLink;
        gPerson* gPersonFrom;
        gPerson* gPersonTo;
        LinkPosition PositionOnFromPerson;
        LinkPosition PositionOnToPerson;
        float offset;

};

#endif // CLINK_H
