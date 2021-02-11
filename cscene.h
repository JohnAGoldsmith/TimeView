#ifndef CSCENE_H
#define CSCENE_H
#include <QGraphicsScene>

class QGraphicsSceneMouseEvent;
class QMenu;
class QPointF;
class QGraphicsLineItem;
class QFont;
class QGraphicsTextItem;
class QColor;

class cLink;

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
    void AddLink(cLink * );
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent * event) override;
private:
    QFont myFont;
    QColor myTextColor;
    QColor myItemColor;
    QColor myLineColor;
    int   timeScale;
    int scaleFactor; // x-axis
    int topPosition;

    
    
};

#endif // CSCENE_H
