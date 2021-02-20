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
    setBackgroundBrush(QColor (250,250,200,127 ));
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
   gPerson* gp;
   QPointF pos = event->scenePos();
   if (itemAt(event->scenePos(),QTransform())){
       gp =  dynamic_cast<gPerson*>( itemAt(event->scenePos(),QTransform() )) ;
       //if (gp) {
       //    qDebug() << "clicked on: " <<  gp->LastName();
       //}
   } //else { qDebug() << "clicked on space";}
   update();
   QGraphicsScene::mousePressEvent(event);


}

void cScene::mouseReleaseEvent(QGraphicsSceneMouseEvent * event){
    gPerson* gp;
    QGraphicsScene::mouseReleaseEvent(event);
    update();
}

void cScene::AddPerson(dPerson * dperson){
    dperson->Ypos ( TopPosition() - dperson->BirthYear() ) ;
    gPerson* gPerson1 = new gPerson(dperson);
    addItem(gPerson1);
    QPointF transformedCoordinates (dperson->Xpos() * ScaleFactor(), dperson->Ypos() * TimeScale());
    gPerson1->setPos(transformedCoordinates);
    dperson->set_gPerson(gPerson1);
}
void cScene::AddLink(cLink * link){
    addItem(link);
    link->setPos(link->GPersonFrom()->scenePos());
    qDebug() << "scene add link"<< link->GPersonFrom()->LastName() << link->GPersonTo()->LastName();
}


