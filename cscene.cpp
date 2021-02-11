#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QTransform>
#include <QPointF>
#include <QGraphicsItem>
#include "cscene.h"
#include "gperson.h"
#include "clink.h"

cScene::cScene(QObject* parent): QGraphicsScene(parent)
{

  // cPerson * person1 = new cPerson(this);
    setBackgroundBrush(QColor (255,0,0,127 ));
    timeScale = 20.0;
    scaleFactor = 25.0;
    topPosition = 2000;

}
cScene::~cScene(){

}




QGraphicsItem * cScene::itemAt(const QPointF   pos, const QTransform & transform ){
    return QGraphicsScene::itemAt(pos, transform);
}

void cScene::mousePressEvent(QGraphicsSceneMouseEvent * event){
   qDebug() << "mouse click in scene";
   QPointF pos = event->scenePos();
   if (itemAt(event->scenePos(),QTransform())){
       gPerson* gp =  dynamic_cast<gPerson*>( itemAt(event->scenePos(),QTransform() )) ;
       qDebug() << "clicked on: " <<  gp->LastName();
   } else { qDebug() << "clicked on space";}
   QGraphicsScene::mousePressEvent(event);
}

void cScene::AddPerson(dPerson * dperson){

    dperson->Ypos ( TopPosition() - dperson->BirthYear() ) ;
    gPerson* gPerson1 = new gPerson(dperson);
    addItem(gPerson1);

    QPointF transformedCoordinates (dperson->Xpos() * ScaleFactor(), dperson->Ypos() * TimeScale());
    gPerson1->setPos(transformedCoordinates);
    dperson->set_gPerson(gPerson1);

    QGraphicsSimpleTextItem * text1 = addSimpleText("x");
    text1->setPos(transformedCoordinates);

}

void cScene::AddLink(cLink * link){

    double fraction1(0.5);

    gPerson * gp1 = link->GPersonFrom();
    gPerson * gp2 = link->GPersonTo();
    QPointF point_start  (gp1->pos().x(), gp1->pos().y());
    QPointF point_end  (gp2->pos().x(), gp2->pos().y());
    //QPointF correction (gp1->CenterX(),0);
    QPointF correction (50,0);
    //point_start += correction;
    //point_end += correction;

    //QGraphicsLineItem * line1 = new QGraphicsLineItem(QLineF(point_start,point_end));
    //addItem(line1);
\
    double verticalDistance = point_end.y() - point_start.y();
    QPointF delta1 (0,fraction1 * verticalDistance);
    QPointF point1 = point_start + delta1;
    QPointF point2 (point_end.x(), point1.y());
    //qDebug() << 61 << gp1->LastName() << point_start.x() << point_start.y() << gp2->LastName() << point_end.x() <<  point_end.y();
    QGraphicsLineItem  *line2, *line3, *line4;

    if (point_start.y() >  point_end.y()){
        double yDiff = -1.0 * (gp1->TopHook().y() - gp2->BottomHook().y());
        line2 = new QGraphicsLineItem(QLineF(point_start,point1));
        line3 = new QGraphicsLineItem(QLineF(point1, point2));
        line4 = new QGraphicsLineItem(QLineF(point2, point_end));

        addItem(line2);
        addItem(line3);
        addItem(line4);
    } else{
        qDebug() << "did not show line"<< gp1->LastName() << gp2->LastName();
    }

}




