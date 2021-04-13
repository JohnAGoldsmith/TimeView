#ifndef CSCENE_H
#define CSCENE_H
#include <QGraphicsScene>
#include <QHash>
#include <QPixmap>
class QGraphicsSceneMouseEvent;
class QMenu;
class QPointF;
class QGraphicsLineItem;
class QFont;
class QGraphicsTextItem;
class QColor;


class cLink;
class gPerson;

class cScene: public QGraphicsScene
{
Q_OBJECT

public:
    explicit cScene(QObject * parent = 0);
    ~cScene();
    QFont font() const { return myFont; }
    QColor textColor() const { return myTextColor; }
    QColor itemColor() const { return myItemColor; }
    QColor lineColor() const { return myLineColor; }
    void setLineColor(const QColor &color);
    void setTextColor(const QColor &color);
    void setItemColor(const QColor &color);
    void setFont(const QFont &font);
    float TimeScale(){return timeScale;}
    float ScaleFactor(){return scaleFactor;}
    int YExpansionFactor() {return y_expansionFactor;}
    int XExpansionFactor() {return x_expansionFactor;}
    int TopPosition() {return topPosition;}
    QGraphicsItem *  itemAt(const QPointF, const QTransform &);
    //void AddLink(cLink * );
    void AddGPerson (gPerson *);
    //void AddGroup( cGroup*);
    QHash<QString, QPixmap*> * Pixmaps() {return pixmaps;}
    QColor Palette(int n);
    QColor getColor(QString key);

    void adjustScenePositions(); // changes the xpos, ypos using its internal x_ and y_expansionfactors

    float ConvertYearToYcoor(float year); // this is used, but it should be eliminated
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent * event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent * event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent * event) override;
    void focusInEvent (QFocusEvent *) override;
private:
    QFont myFont;
    QColor myTextColor;
    QColor myItemColor;
    QColor myLineColor;
    QList<QColor> myPalette;
    float   timeScale;
    float scaleFactor;
    float x_expansionFactor; // x-axis
    float y_expansionFactor;
    int topPosition;
    gPerson* selectedPerson;
    QHash<QString, QPixmap*> *  pixmaps;

signals:
    //void sendToLineEdit(QString message);

public slots:
    void changeHorizontalScale(int);


};

#endif // CSCENE_H
