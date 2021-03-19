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
        cLink(bool, QStringList &); // used for legacy spreadsheets
        ~cLink();
        QRectF boundingRect() const;
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
        QString display() const;
        void write(QJsonObject & json) const;
        void read(QJsonObject & json);
        QString export2csv();
        void importFromCSV(QString line);
        void importFromCSVlegacy(QString line);

        QString getKey() const ;
        QString getFromKey(){return fromPersonKey;}
        QString getToKey(){return toPersonKey;}
        gPerson* GPersonFrom()const {return gPersonFrom;}
        gPerson* GPersonTo() const {return gPersonTo;}
        void setPersonFrom(gPerson*);
        void setPersonTo(gPerson*);

        void attachPersons(gPerson *, gPerson *);

        void BottomOffset(float f) ;
        void TopOffset(float f) ;
        double TopOffset() {return topOffset;}
        double BottomOffset() {return bottomOffset;}

        QString GetPositionOnFromPerson(){return PositionOnFromPerson;}
        QString GetPositionOnToPerson(){return PositionOnToPerson;}

        QString NatureOfLink() {return natureOfLink;}

        void Invisible() {visible = false;}
        void Visible() {visible = true;}
        void setGrayed(bool value) {grayed = value;}
        bool Chosen() {return chosen;}
        void setChosen(){chosen = true;}
        void setUnChosen() {chosen = false;}

        void shortenProportion1() {proportion1 *= 0.90;};
        void lengthenProportion1() {proportion1 *= 1.10;};
        float GetProportion1() {return proportion1;}
    private:
        QString fromPersonKey;
        QString toPersonKey;
        QString natureOfLink;
        gPerson* gPersonFrom;
        gPerson* gPersonTo;
        QString PositionOnFromPerson;
        QString PositionOnToPerson;
        float bottomOffset;
        float topOffset;
        bool grayed;
        bool visible;
        bool chosen;
        float proportion1;
        QPointF startPoint;
        QPointF endPoint;

};

#endif // CLINK_H
