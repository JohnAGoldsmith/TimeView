#ifndef COLUMNARSCENE_H
#define COLUMNARSCENE_H

#include "cscene.h"
#include <QObject>

class columnarScene : public cScene
{
public:
    columnarScene();
    void addToColumnar(gPerson*);
    void sortColumnar();
    void displayColumnar();

private:
    QList<gPerson*> columnar;
};

#endif // COLUMNARSCENE_H
