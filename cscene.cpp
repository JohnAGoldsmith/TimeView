#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsSceneMouseEvent>
#include <QTransform>
#include <QPointF>
#include <QGraphicsItem>
#include <QKeyEvent>
#include <QHash>
#include <QPixmap>
#include <QTableWidget>
#include <QColor>
#include "math.h"
#include "cscene.h"
#include "gperson.h"
#include "clink.h"
#include "clineedit.h"

cScene::cScene(QObject* parent): QGraphicsScene(parent)
{

  // cPerson * person1 = new cPerson(this);
    setBackgroundBrush(QColor (250,250,200,127 ));
    y_expansionFactor = 1.5;
    x_expansionFactor = 1.5;
    topPosition = 2000;
    timeScale = 20.0;  // this is used as the basic factor to go from years to Scene position. We change y_expansionFactor if we want to adjust the display.
    scaleFactor = 50.0;


    myPalette.append(QColor(243,114,87)); // burnt orange 0
    myPalette.append(QColor(246,141,92)); //
    myPalette.append(QColor(244,210,122)); //

    myPalette.append(QColor(81,114,129)); //  3
    myPalette.append(QColor(120,149,162)); // 4
    myPalette.append(QColor(175,193,204)); // 5

    //greens
    myPalette.append(QColor(183,221,176)); // light green 6
    myPalette.append(QColor(11,244,70)); // bright green 7
    myPalette.append(QColor(57,114,73)); // dark green 8

    //blues
    myPalette.append(QColor(150 ,221,255)); //  9  light blue
    myPalette.append(QColor(1 ,191 ,255)); // 10 medium blue
    myPalette.append(QColor( 0,74 ,255)); // 11 dark blue,

    //yellows
    myPalette.append(QColor(255 ,251 ,5)); //  12 light yellow
    myPalette.append(QColor(255 ,229, 5)); //  13 medium yellow
    myPalette.append(QColor(255 ,201,5)); //  14 orange


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
QColor cScene::getColor(QString key){
    if (key == "linguist"){
        return myPalette[0];
    }
    if (key == "philosopher"){
        return myPalette[9];
    }
    if (key == "psychologist"){
        return myPalette[12];
    }
    if (key == "Phil-psych" || key == "philpsych"){
        return myPalette[7];
    }
    if (key == "logician"){
        return myPalette[3];
    }
    if (key == "mathematician"){
        return myPalette[4];
    }
    if (key == "sociologist"){
        return myPalette[6];
    }
    if (key == "focus" || key == "grayed"){
        return QColor(Qt::gray);
    }
    return myPalette[5];
}

QColor cScene::Palette(int n) {
    if (n< myPalette.size()){
        return myPalette[n];
    } else{
        return Qt::black;
    }
}
void cScene::changeHorizontalScale(float factor){
    foreach(QGraphicsItem* item, items()){
       item->setPos(item->pos().x() * factor, item->pos().y());
       item->update();
    }
    update();
    if (focusItem()){
        views().constFirst()->centerOn(focusItem());
    }
}
void cScene::changeVerticalScale(float factor){
    foreach(QGraphicsItem* item, items()){
       item->setPos(item->pos().x(), item->pos().y() * factor);
       item->update();
    }
    update();
    if (focusItem()){
        views().constFirst()->centerOn(focusItem());
    }
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

// deprecated.
void cScene::adjustScenePositions()  // changes the xpos, ypos using its internal x_ and y_expansionfactors
{
    foreach (QGraphicsItem* item, items() ){
      QPointF coor = item->pos();
      qDebug() << coor.x() << coor.y();
      item->setPos(coor.x() * x_expansionFactor, coor.y() * y_expansionFactor);
    }

}
/*
void cScene::AddLink(cLink * link){
    addItem(link);
    //link->setPos(link->GPersonFrom()->scenePos());
    link->setPos(link->GPersonFrom()->pos());
}
*/
