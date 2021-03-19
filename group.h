#ifndef GROUP_H
#define GROUP_H
#include <QGraphicsRectItem>

class cGroup: public QGraphicsItem// QGraphicsRectItem
{
public:
    cGroup(QStringList);
    cGroup(QString key, QString name, float x, float y, float h, float w );
    ~cGroup();
    QRectF boundingRect() const override;

   void paint( QPainter * painter, const QStyleOptionGraphicsItem * option,
                QWidget * widget);

    QString Key() {return key;}
    float X() {return scene_x;}
    float Y() {return scene_y;}
    void setY(float val){scene_y = val;}
    QString export2csv();


    void mousePressEvent(QGraphicsSceneMouseEvent * event);

private:
    QString key;
    QString name;
    float scene_x;
    float scene_y;
    float height;
    float width;

    QRectF  myBoundingRect;

};

#endif // GROUP_H
