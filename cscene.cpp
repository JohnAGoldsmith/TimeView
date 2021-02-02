#include "cscene.h"
#include "cperson.h"

cScene::cScene(QObject* parent): QGraphicsScene(parent)
{

  // cPerson * person1 = new cPerson(this);
    setBackgroundBrush(QColor (255,0,0,127 ));

}
cScene::~cScene(){

}
