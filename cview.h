#ifndef CVIEW_H
#define CVIEW_H

#include <QGraphicsView>
#include <QObject>
#include <QWheelEvent>
#include "cscene.h"
class cView : public QGraphicsView
{
public:
    cView();
    cView(cScene*);
    void mousePressEvent(QMouseEvent *event) override;


 protected Q_SLOTS:
    void wheelEvent(QWheelEvent *event);

};

#endif // CVIEW_H
