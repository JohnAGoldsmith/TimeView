#include <QMouseEvent>
#include <QtDebug>
#include <QAbstractScrollArea>
#include <QScrollBar>
#include "cview.h"
#include "cscene.h"

cView::cView()
{
   setDragMode(QGraphicsView::ScrollHandDrag);
}

cView::cView(cScene* scene):QGraphicsView(scene){

}
void cView::cView::mousePressEvent(QMouseEvent *event){
    qDebug() << "mouse pressed for view" << items(event->pos()).size() ;
    QGraphicsView::mousePressEvent(event);
}
void cView::wheelEvent(QWheelEvent *event)
{
  if (event->modifiers() & Qt::ShiftModifier){
    if(event->delta() > 0)
       //scale(1.25, 1.25);
      QAbstractScrollArea::horizontalScrollBar()->setValue(horizontalScrollBar()->value() + 50);
    else
       //scale(0.8, 0.8);
       QAbstractScrollArea::horizontalScrollBar()->setValue(horizontalScrollBar()->value() - 50);

  }
  else if (event->modifiers() & Qt::AltModifier){
    if(event->delta() > 0)
       //scale(1.25, 1.25);
      QAbstractScrollArea::verticalScrollBar()->setValue(verticalScrollBar()->value() + 40);
    else
       //scale(0.8, 0.8);
       QAbstractScrollArea::verticalScrollBar()->setValue(verticalScrollBar()->value() - 40);

  }
  else{
    if(event->delta() > 0)
       scale(1.25, 1.25);
    else
       scale(0.8, 0.8);
  }
}
