#ifndef GPERSON_H
#define GPERSON_H
#include <QGraphicsRectItem>
#include <QGraphicsItem>
#include <QGraphicsScene>
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
    void Move(int x, int  y);
    float Xpos(){return xpos;}
    float Ypos(){return ypos;}
    QString LastName() {return lastName;}
    //QPointF BottomHook();
    //QPointF TopHook();
    dPerson* getDPerson() const {return dataPerson;}
    float CenterX();
    float Height() {return height;}
    float GetNameWidth()const;
    float GetDatesWidth() const ;
    void AppendLink (cLink * link);


protected:
    void mousePressEvent(QGraphicsSceneMouseEvent * event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent * event) override;

private:
    cScene * scene;
    dPerson * dataPerson;
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
    QList<cLink*> myLinks;
    //double scaleFactor; // this should be in cScene, but I need it in paint for this class.
    //double timeScale; //ditto.
};



#endif // GPERSON_H
