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



    void rememberPos(QPointF);
    //double Width() {return width;}
    void SetY(int y) {ypos = y;}
    //float Xpos(){return xpos;}
    float Ypos(){return ypos;}
    QString LastName() {return lastName;}
    dPerson* getDPerson() const {return dataPerson;}

    float Height() {return height;}
    float GetNameWidth(QPainter * )const;
    //float GetNameWidth( )const;
    float GetDatesWidth(QPainter * ) const ;
    void AppendLink (cLink * link);
    QList<cLink*> * GetLinks() {return & myLinks;}
    QList<cLink*> * GetTopLinks() {return & topLinks;}
    QList<cLink*> * GetBottomLinks() {return & bottomLinks;}
    void SortLinks();
    QString Key();

    // Json:
    void read(const QJsonObject & json);
    void write(QJsonObject & json) const;


protected:
    void mousePressEvent(QGraphicsSceneMouseEvent * event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent * event) override;

private:
    cScene * scene;
    dPerson * dataPerson;
    QFont *  myFont;
    QString key;
    float xpos;
    float ypos;
    float height;
    QString firstName;
    QString lastName;
    QRectF personBoundingRect;
    QList<cLink*> myLinks;
    QList<cLink*> topLinks;
    QList<cLink*> bottomLinks;

};



#endif // GPERSON_H
