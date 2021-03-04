#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QTransform>
#include <QPointF>
#include <QGraphicsItem>
#include <QKeyEvent>
#include <QHash>
#include <QPixmap>
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
    pixmaps = new QHash<QString, QPixmap*>();
    QPixmap * pixmap;
    pixmap = new QPixmap("./blue.jpg");
    pixmaps->insert("blue", pixmap);
    pixmap = new QPixmap("./lightbrown.jpg");
    pixmaps->insert("lightbrown", pixmap);
    pixmap = new QPixmap("./apricot.webp");
    pixmaps->insert("apricot", pixmap);
    pixmap = new QPixmap("./darkredwood.webp");
    pixmaps->insert("darkredwood", pixmap);
    pixmap = new QPixmap("./blueslate.jpg");
    pixmaps->insert("blueslate", pixmap);
    pixmap = new QPixmap("./dark_light_wood.webp");
    pixmaps->insert("darklightwood", pixmap);
    pixmap = new QPixmap("./red.png");
    pixmaps->insert("red", pixmap);


}
cScene::~cScene(){
    if (pixmaps){
        foreach (QPixmap* pixmap, *pixmaps)
           delete pixmap;
    }
    delete pixmaps;

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


