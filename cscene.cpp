#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QTransform>
#include <QPointF>
#include <QGraphicsItem>
#include <QKeyEvent>
#include <QTableWidget>
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
   QGraphicsScene::mousePressEvent(event);
}
void cScene::mouseMoveEvent(QGraphicsSceneMouseEvent* event){
    QGraphicsScene::mouseReleaseEvent(event);
    update();
}


void cScene::mouseReleaseEvent(QGraphicsSceneMouseEvent * event){
    QGraphicsScene::mouseReleaseEvent(event);
}
void cScene::focusInEvent(QFocusEvent * event){

}
void cScene::AddLink(cLink * link){
    addItem(link);
    //link->setPos(link->GPersonFrom()->scenePos());
    link->setPos(link->GPersonFrom()->pos());
}


