#ifndef GROUP_H
#define GROUP_H
#include <QGraphicsRectItem>

class cGroup: public QGraphicsItem// QGraphicsRectItem
{
public:
    cGroup(); // used in creating from json file;
    cGroup(QStringList);
    cGroup(QString key, QString name, float x, float y, float h, float w );
    ~cGroup();
    QRectF boundingRect() const override;

   void paint( QPainter * painter, const QStyleOptionGraphicsItem * option,
                QWidget * widget) override;

    QString Key() {return key;}
    QPointF MemoryOfPos();
    float X() {return scene_x;}
    float Y() {return scene_y;}
    void setY(float val){scene_y = val;}
    QString export2csv();
    void write(QJsonObject & json) const;
    void read(const QJsonObject &);

protected:

    //void mouseMoveEvent(QGraphicsSceneMouseEvent * event) override;
    //void mousePressEvent(QGraphicsSceneMouseEvent * event) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event) override;

private:
    QString key;
    QString name;
    float scene_x;
    float scene_y;
    float height;
    float width;

    QRectF  myBoundingRect;

public slots:
    void changeHeight(int newHeight);
    void changeWidth( int newWidth);

};

#endif // GROUP_H
