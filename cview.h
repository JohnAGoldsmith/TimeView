#ifndef CVIEW_H
#define CVIEW_H

#include <QGraphicsView>
#include <QObject>
#include "cscene.h"
class cView : public QGraphicsView
{
public:
    cView();
    cView(cScene*);
    void mousePressEvent(QMouseEvent *event) override;
};

#endif // CVIEW_H
