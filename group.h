#ifndef GROUP_H
#define GROUP_H
#include <QGraphicsRectItem>

class cGroup: public QGraphicsRectItem
{
public:
    cGroup(QStringList);
    ~cGroup();
    QRectF boundingRect() const override;

    void paint( QPainter * painter, const QStyleOptionGraphicsItem * option,
                QWidget * widget);

    QString Key() {return key;}

private:
    QString key;
    QString name;
    float x;
    float y;
    float width;
    float height;
};

#endif // GROUP_H
