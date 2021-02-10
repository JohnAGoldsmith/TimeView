#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QTransform>
#include <QPointF>
#include <QGraphicsItem>
#include "cscene.h"
#include "gperson.h"

cScene::cScene(QObject* parent): QGraphicsScene(parent)
{

  // cPerson * person1 = new cPerson(this);
    setBackgroundBrush(QColor (255,0,0,127 ));
    timeScale = 20;
    scaleFactor = 20;
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
       qDebug() << gp->LastName();
   } else { qDebug() << "clicked on space";}
   QGraphicsScene::mousePressEvent(event);
}
