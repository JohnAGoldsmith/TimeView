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
        QString display() const;
        void write(QJsonObject & json) const;
        void read(QJsonObject & json);

        QString getFromKey(){return fromPersonKey;}
        QString getToKey(){return toPersonKey;}
        //void attachScene(cScene*);
        void attachGraphicalPersons(gPerson *, gPerson *);
        gPerson* GPersonFrom()const {return gPersonFrom;}
        gPerson* GPersonTo() const {return gPersonTo;}
        void BottomOffset(float f) ;
        void TopOffset(float f) ;
        double TopOffset() {return topOffset;}
        double BottomOffset() {return bottomOffset;}
        QString GetPositionOnFromPerson(){return PositionOnFromPerson;}
        QString GetPositionOnToPerson(){return PositionOnToPerson;}
        QString NatureOfLink() {return natureOfLink;}
        void Invisible() {visible = false;}
        void Visible() {visible = true;}
        bool Chosen() {return chosen;}
        void setChosen(){chosen = true;}
        void setUnChosen() {chosen = false;}
        void setPersonFrom(gPerson*);
        void setPersonTo(gPerson*);
    private:
        QString fromPersonKey;
        QString toPersonKey;
       // cPath  * path;
        QString natureOfLink;
        gPerson* gPersonFrom;
        gPerson* gPersonTo;
        QString PositionOnFromPerson;
        QString PositionOnToPerson;
        float bottomOffset;
        float topOffset;
        bool visible;
        bool chosen;
        float proportion1;
        QPointF startPoint;
        QPointF endPoint;

};

#endif // CLINK_H
