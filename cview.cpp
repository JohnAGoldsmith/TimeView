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

void cView::wheelEvent(QWheelEvent *event)
{
  if (event->modifiers() & Qt::ShiftModifier){
    if(event->delta() > 0)
       //scale(1.25, 1.25);
      QAbstractScrollArea::horizontalScrollBar()->setValue(horizontalScrollBar()->value() + 100);
    else
       //scale(0.8, 0.8);
       QAbstractScrollArea::horizontalScrollBar()->setValue(horizontalScrollBar()->value() - 100);

  }
  else if (event->modifiers() & Qt::AltModifier){
    if(event->delta() > 0)
       //scale(1.25, 1.25);
      QAbstractScrollArea::verticalScrollBar()->setValue(verticalScrollBar()->value() + 100);
    else
       //scale(0.8, 0.8);
       QAbstractScrollArea::verticalScrollBar()->setValue(verticalScrollBar()->value() - 100);

  }
  else{
    const ViewportAnchor anchor = transformationAnchor();
    //setResizeAnchor( QGraphicsView::AnchorUnderMouse );
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    if(event->delta() > 0)
       scale(1.25, 1.25);
    else
       scale(0.8, 0.8);
    //setResizeAnchor(anchor);
    setTransformationAnchor(anchor);
  }
}
/*           Move entire scene
void cView::mousePressEvent(QMouseEvent* event)
{
    qDebug() << "In view mouse press event";
    if (event->button() == Qt::RightButton)
    {
        // Store original position.
        temp_x_pos = event->x();
        temp_y_pos = event->y();
    }
      QGraphicsView::mousePressEvent(event);
}

void cView::mouseMoveEvent(QMouseEvent* event)
{
    qDebug() << "mainwindow mouse";
    setTransformationAnchor(QGraphicsView::NoAnchor);
    if (event->buttons() & Qt::RightButton)
    {
        QPointF oldp = mapToScene(temp_x_pos, temp_y_pos);
        QPointF newp = mapToScene(event->pos());
        QPointF translation = newp - oldp;

        translate(translation.x(), translation.y());

        temp_x_pos = event->x();
        temp_y_pos = event->y();
    }
    QGraphicsView::mouseMoveEvent(event);
}                                                     */
