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
#include "math.h"
#include "cscene.h"
#include "gperson.h"
#include "clink.h"
#include "clineedit.h"

cScene::cScene(QObject* parent): QGraphicsScene(parent)
{

  // cPerson * person1 = new cPerson(this);
    setBackgroundBrush(QColor (250,250,200,127 ));
    timeScale = 20.0;
    scaleFactor = 25.0;
    topPosition = 2000;
    pixmaps = new QHash<QString, QPixmap*>();
    QPixmap * pixmap;
    pixmap = new QPixmap("./colors/blue.jpg");
    pixmaps->insert("blue", pixmap);
    pixmap = new QPixmap("./colors/yellow.jpg");
    pixmaps->insert("yellow", pixmap);

    pixmap = new QPixmap("./colors/lightbrown.jpg");
    pixmaps->insert("lightbrown", pixmap);
    pixmap = new QPixmap("./colors/apricot.webp");
    pixmaps->insert("apricot", pixmap);
    pixmap = new QPixmap("./colors/darkredwood.webp");
    pixmaps->insert("darkredwood", pixmap);
    pixmap = new QPixmap("./colors/blueslate.jpg");
    pixmaps->insert("blueslate", pixmap);
    pixmap = new QPixmap("./colors/dark_light_wood.webp");
    pixmaps->insert("darklightwood", pixmap);
    pixmap = new QPixmap("./colors/red.png");
    pixmaps->insert("red", pixmap);

    QObject::connect (this, SIGNAL(sendToLineEdit(QString)), parent, SLOT(cLineEdit.setText(QString)));
}
cScene::~cScene(){
    if (pixmaps){
        foreach (QPixmap* pixmap, *pixmaps)
           delete pixmap;
    }
    delete pixmaps;
}

void cScene::changeHorizontalScale(int value){
    float factor = 1.0 + value/20.0;
    //qDebug() << " factor " << factor;
    foreach(QGraphicsItem* item, items()){
       item->setPos(item->pos().x() * factor, item->pos().y() * factor);
       item->update();
    }
    update();
}

float cScene::ConvertYearToYcoor(float year){
    return timeScale * (topPosition - year);
}

QGraphicsItem * cScene::itemAt(const QPointF   pos, const QTransform & transform ){
    return QGraphicsScene::itemAt(pos, transform);
}

void cScene::mousePressEvent(QGraphicsSceneMouseEvent * event){
   //qDebug() << "In scene, mouse event";
   QGraphicsScene::mousePressEvent(event);
   update();
}
void cScene::mouseMoveEvent(QGraphicsSceneMouseEvent* event){
    //qDebug() << "In scene, mouse move";
    QGraphicsScene::mouseMoveEvent(event);
    update();
}
void cScene::mouseReleaseEvent(QGraphicsSceneMouseEvent * event){
    QGraphicsScene::mouseReleaseEvent(event);
    update();
}
void cScene::focusInEvent(QFocusEvent * event){

}
/*
void cScene::AddLink(cLink * link){
    addItem(link);
    //link->setPos(link->GPersonFrom()->scenePos());
    link->setPos(link->GPersonFrom()->pos());
}
*/
