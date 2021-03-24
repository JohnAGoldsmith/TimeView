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
    int TimeScale() {return timeScale;}
    int ScaleFactor() {return scaleFactor;}
    int TopPosition() {return topPosition;}
    QGraphicsItem *  itemAt(const QPointF, const QTransform &);
    //void AddLink(cLink * );
    void AddGPerson (gPerson *);
    //void AddGroup( cGroup*);
    QHash<QString, QPixmap*> * Pixmaps() {return pixmaps;}
    void changeHorizontalScale(int);

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
    int   timeScale;
    int scaleFactor; // x-axis
    int topPosition;
    gPerson* selectedPerson;
    QHash<QString, QPixmap*> *  pixmaps;
signals:
    //void sendToLineEdit(QString message);

};

#endif // CSCENE_H
