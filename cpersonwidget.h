#ifndef CPERSONWIDGET_H
#define CPERSONWIDGET_H
#include "gperson.h"

#include <QWidget>

class cPersonWidget : public QWidget
{
public:
    cPersonWidget();

    gPerson *   createPerson(cScene*);
};

#endif // CPERSONWIDGET_H
