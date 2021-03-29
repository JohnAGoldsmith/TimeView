#ifndef GPERSON_H
#define GPERSON_H
#include <QGraphicsRectItem>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QList>
#include <QFocusEvent>
#include <QEvent>
#include <QStringList>
//#include "data.h"
class cScene;
class cLink;

class gPerson : public QGraphicsItem
{


public:
    gPerson();
    gPerson(QStringList   data);
    gPerson(bool dummy, QStringList data);  // only for legacy CSV input
    ~gPerson();
    QRectF boundingRect() const override;

    void paint( QPainter * painter, const QStyleOptionGraphicsItem * option,
                QWidget * widget)override;
    bool Pressed;

    cScene * Scene() {return scene;}  /* This really shouldn't be necessary; it should inherit this from QGraphicsItem */
    void Scene(cScene * iscene) {scene = iscene;}

    void rememberPos(QPointF);
    QPointF GetMemoryOfScreenPosition();

    float BirthYear() {return birthYear;}
    float DeathYear() {return deathYear;}
    QString LastName() {return lastName;}
    QString FirstName() {return firstName;}
    QString Profession1() {return profession1;}

    float Height() {return height;}
    float GetNameWidth(QPainter * )const;
    float GetNameHeight(QPainter *) const;
    float GetDatesWidth(QPainter * ) const ;
    float GetDatesHeight(QPainter * ) const ;
    float GetTextHeight(QPainter* ) const;
    float GetTextWidth (QPainter*, QString) const;

    void AppendLinkAndResortEdges (cLink * link); // not used with data from Json file, which remembers.
    void AppendLinkAndAddLinkKeyToSide (cLink * link);
    //void AppendTopLink (cLink*);
    void AppendBottomLink(cLink*);
    void AppendLinkKey(QString linkKey);
    void AppendTopLinkKey(QString);
    void AppendRightLinkKey(QString);
    void AppendBottomLinkKey(QString);
    void AppendLeftLinkKey(QString);
    void AppendLink(cLink* link) {myLinks.append(link);};

    QList<cLink*> * GetLinks() {return & myLinks;}
    //QList<cLink*> * GetTopLinks() {return & topLinks;} //remove
    QStringList * GetTopLinkKeys() {return & topLinkKeys;}
    QStringList * GetRightLinkKeys() {return & rightLinkKeys;}
    QStringList * GetBottomLinkKeys() {return & bottomLinkKeys;}
    QStringList * GetLeftLinkKeys() {return & leftLinkKeys;}
    QList<cLink*> * GetBottomLinks() {return & bottomLinks;} //remove
    void SetSelectedLink(cLink * link) {selectedLink = link;}
    cLink* SelectedLink() {return selectedLink;}
    void SetKey2Link(QString key, cLink* link);
    cLink* Key2Link(QString key) {return myKeys2Links[key];}

    void UnselectAllLinks();
    void SortLinksOnEachSide();
    void ShiftLink();

    QString Key(){return key;}
    void setKey(QString thiskey){key = thiskey;}
    void setWidth(float w){width = w;}
    float Width(){return width;}
    float X_fromspreadsheet() {return x_fromspreadsheet;}

    bool Grayed() {return grayed;}
    void setGrayed(bool value) {grayed = value;}



    // Json:
    void read(const QJsonObject & json);
    void write(QJsonObject & json) const;

    QString export2CSV();

    void AddLinkKeyToSide(cLink* link);
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent * event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent * event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent * event) override;

    void mouseDoubleClickEvent (QGraphicsSceneMouseEvent * event) override;
    void focusInEvent(QFocusEvent * event)   override;
    void focusOutEvent(QFocusEvent* event) override;

    void keyPressEvent (QKeyEvent * event) override;

private:
    cScene * scene;
    QFont *  myFont;
    QString firstName;
    QString lastName;
    int birthYear;
    int deathYear;
    QString key;
    float xpos;              // used only for input  not during the program
    float ypos;              // used only for input  not during the program
    float x_fromspreadsheet; // used only for legacy operations
    float y_fromspreadsheet;
    float height;
    float width;
    float margin;


    QString profession1;
    QString profession2;
    QRectF personBoundingRect;
    QList<cLink*> myLinks;// keep this up-to-date, matching order of links on the four edges
    QHash<QString, cLink*> myKeys2Links;
    //QList<cLink*> topLinks; //remove
    QList<cLink*> bottomLinks;//remove
    QList<cLink*> leftLinks;//remove
    QList<cLink*> rightLinks;//remove
    QStringList topLinkKeys;
    QStringList bottomLinkKeys;
    QStringList leftLinkKeys;
    QStringList rightLinkKeys;
    bool grayed;
    //bool visible;
    //bool limbo;
    cLink *  selectedLink;
    QString selectedLinkKey;


};



#endif // GPERSON_H
