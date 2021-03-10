#ifndef CVIEW_H
#define CVIEW_H

#include <QGraphicsView>
#include <QObject>
#include <QWheelEvent>
#include <QMouseEvent>
#include "cscene.h"
class cView : public QGraphicsView
{
public:
    cView();
    cView(cScene*);
    /*          The presence of the next two functions overrode the dragging of QGraphicsItems, unfortunately       */
    //void mousePressEvent(QMouseEvent *event) override;
    //void mouseMoveEvent(QMouseEvent* event);

private:
    int temp_x_pos;
    int temp_y_pos;




 protected Q_SLOTS:
    void wheelEvent(QWheelEvent *event);

};

#endif // CVIEW_H
