#include <QMouseEvent>
#include <QtDebug>
#include "cview.h"
#include "cscene.h"

cView::cView()
{

}

cView::cView(cScene* scene):QGraphicsView(scene){

}
void cView::cView::mousePressEvent(QMouseEvent *event){
    qDebug() << "mouse pressed for view" << items(event->pos()).size() ;
    QGraphicsView::mousePressEvent(event);
}
