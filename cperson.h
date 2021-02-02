#ifndef CPERSON_H
#define CPERSON_H
#include <QGraphicsRectItem>
#include <QGraphicsItem>
#include <QGraphicsScene>
class cScene;

class cPerson : public QGraphicsRectItem
{
public:
    cPerson(cScene *, QString FirstName, QString LastName, int x = 30, int y = 30);
    void setY(int y) {ypos = y;}
    void move(int x, int  y);


private:
    cScene * scene;
    QGraphicsRectItem * box;
    QGraphicsSimpleTextItem * nameItem;
    float xpos;
    float ypos;
    float height;
    float width;
    QString firstName;
    QString lastName;
};



#endif // CPERSON_H
