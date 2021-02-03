#include "cscene.h"
#include "gperson.h"

cScene::cScene(QObject* parent): QGraphicsScene(parent)
{

  // cPerson * person1 = new cPerson(this);
    setBackgroundBrush(QColor (255,0,0,127 ));
    timeScale = 20;
    scaleFactor = 20;
    topPosition = 2000;

}
cScene::~cScene(){

}
