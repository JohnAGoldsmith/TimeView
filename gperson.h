#ifndef GPERSON_H
#define GPERSON_H
#include <QGraphicsRectItem>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QList>
#include "dataperson.h"

class cScene;
class cLink;

class gPerson : public QGraphicsItem
{

    friend class dPerson;
public:
    gPerson();
    gPerson(dPerson * dPerson);
    ~gPerson();
    QRectF boundingRect() const override;

    void paint( QPainter * painter, const QStyleOptionGraphicsItem * option,
                QWidget * widget);
    bool Pressed;

    double Width() {return width;}
    void SetY(int y) {ypos = y;}
    float Xpos(){return xpos;}
    float Ypos(){return ypos;}
    QString LastName() {return lastName;}
    QPointF BottomHook() {return bottomHook;}
    QPointF TopHook(){return topHook;}
    dPerson* getDPerson() const {return dataPerson;}
    float CenterX( QPainter * painter );
    float Height() {return height;}
    float GetNameWidth(QPainter * )const;
    float GetDatesWidth(QPainter * ) const ;
    void AppendLink (cLink * link);
    QList<cLink*> * GetLinks() {return & myLinks;}
    QList<cLink*> * GetTopLinks() {return & topLinks;}
    QList<cLink*> * GetBottomLinks() {return & bottomLinks;}
    void SortLinks();


protected:
    void mousePressEvent(QGraphicsSceneMouseEvent * event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent * event) override;

private:
    cScene * scene;
    dPerson * dataPerson;
    QFont *  myFont;

    QGraphicsRectItem * box;
    QGraphicsSimpleTextItem * nameItem;
    QGraphicsSimpleTextItem * yearsItem;
    float xpos;
    float ypos;
    float height;
    float width;
    double margin;
    QString firstName;
    QString lastName;

    QRectF personBoundingRect;
    float centerX;
    QPoint bottomHook;
    QPoint topHook;
    QList<cLink*> myLinks;

    QList<cLink*> topLinks;
    QList<cLink*> bottomLinks;

};



#endif // GPERSON_H
