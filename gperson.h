#ifndef GPERSON_H
#define GPERSON_H
#include <QGraphicsRectItem>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QList>
#include <QFocusEvent>
#include "dataperson.h"

class cScene;
class cLink;

class gPerson : public QGraphicsItem
{

    friend class dPerson;
public:
    gPerson();
    gPerson(dPerson * dPerson);
    gPerson(QStringList   data);
    ~gPerson();
    QRectF boundingRect() const override;

    void paint( QPainter * painter, const QStyleOptionGraphicsItem * option,
                QWidget * widget);
    bool Pressed;



    void rememberPos(QPointF);
    //double Width() {return width;}
    void SetY(int y) {ypos = y;}
    float Xpos(){return xpos;}
    float Ypos(){return ypos;}
    QString LastName() {return lastName;}
    QString FirstName() {return firstName;}
    //dPerson* getDPerson() const {return dataPerson;}
    float BirthYear() {return birthYear;}
    float DeathYear() {return deathYear;}
    QString Profession1() {return profession1;}

    float Height() {return height;}
    //void Height(float h ) {height = h;}
    float GetNameWidth(QPainter * )const;
    float GetNameHeight(QPainter *) const;
    float GetDatesWidth(QPainter * ) const ;
    float GetDatesHeight(QPainter * ) const ;
    float GetTextHeight(QPainter* ) const;
    float GetTextWidth (QPainter*, QString) const;
    void AppendLink (cLink * link);
    QList<cLink*> * GetLinks() {return & myLinks;}
    QList<cLink*> * GetTopLinks() {return & topLinks;}
    QList<cLink*> * GetBottomLinks() {return & bottomLinks;}
    void ShowSelectedLinkSet();
    void SetSelectedLink(cLink * link) {selectedLink = link;}
    void UnselectAllLinks();
    void SortLinks();

    QString Key(){return key;}
    void setKey(QString thiskey){key = thiskey;}
    void setWidth(float w){width = w;}
    float Width(){return width;}
    float X_fromspreadsheet() {return x_fromspreadsheet;}
   //void LinkToDPersonBidirectional(dPerson* dperson);
    bool Limbo() {return limbo;}
    bool Visible() {return visible;}
    bool setVisible(bool b) {visible = b;}


    // Json:
    void read(const QJsonObject & json);
    void write(QJsonObject & json) const;


protected:
    void mousePressEvent(QGraphicsSceneMouseEvent * event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent * event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent * event) override;

    void mouseDoubleClickEvent (QGraphicsSceneMouseEvent * event) override;
    void focusInEvent(QFocusEvent * event)   override;
    void focusOutEvent(QFocusEvent* event) override;

private:
    cScene * scene;
    //dPerson * dataPerson;
    QFont *  myFont;
    QString key;
    float xpos;
    float ypos;
    float x_fromspreadsheet;
    float height;
    float width;
    float margin;
    QString firstName;
    QString lastName;
    int birthYear;
    int deathYear;
    QString profession1;
    QString profession2;
    QRectF personBoundingRect;
    QList<cLink*> myLinks;
    QList<cLink*> topLinks;
    QList<cLink*> bottomLinks;
    bool visible;
    bool limbo;
    QString selectedLinkSet; // should be an enum: options are top, bottom, left, right.
    cLink *  selectedLink;
};



#endif // GPERSON_H
