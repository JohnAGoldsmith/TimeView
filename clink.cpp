#include <QGraphicsLineItem>
#include <QDebug>
#include "clink.h"
#include "gperson.h"
#include "cscene.h"

cLink::cLink()
{

}
cLink::cLink(QStringList data){
    fromPersonKey = data[1];
    toPersonKey = data[2];
    if (data.size() >= 11 && data[10].length() > 0){
        natureOfLink = data[10];
    }

}
void cLink::attachScene(cScene* scene){
 double fraction1(0.5);
 double fraction2(0.1);
 double fraction3(0.4);
 QGraphicsLineItem * line1, *line2, *line3, *line4;
 line1 = new QGraphicsLineItem(QLineF(gPersonFrom->TopHook(),gPersonTo->BottomHook()));
 qDebug() << 22 <<  gPersonFrom->LastName() << gPersonFrom->TopHook().y() << gPersonTo->LastName() << gPersonTo->BottomHook().y();
 if (gPersonTo->BottomHook().y() < gPersonFrom->TopHook().y()){
     double yDiff = -1.0 * (gPersonFrom->TopHook().y() - gPersonTo->BottomHook().y());
     QPointF point2(gPersonFrom->TopHook().rx(),gPersonTo->TopHook().ry() - yDiff * fraction1);
     QPointF point3(gPersonTo->TopHook().rx(),gPersonTo->TopHook().ry() - yDiff * fraction1);

     line2 = new QGraphicsLineItem(QLineF(gPersonFrom->TopHook(),point2));
     line3 = new QGraphicsLineItem(QLineF(point2,point3));
     line4 = new QGraphicsLineItem(QLineF(point3, gPersonTo->BottomHook()));
     scene->addItem(line2);
     scene->addItem(line3);
     scene->addItem(line4);
 }

}

void cLink::attachGraphicalPersons(gPerson * person1, gPerson * person2){
 gPersonFrom = person1;
 gPersonTo = person2;
}
