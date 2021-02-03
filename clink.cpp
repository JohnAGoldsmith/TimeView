#include <QGraphicsLineItem>
#include "clink.h"
#include "gperson.h"
#include "cscene.h"

cLink::cLink()
{

}
cLink::cLink(QStringList data){
    fromPersonKey = data[1];
    toPersonKey = data[2];
    if (data.size() >= 11 && data[10].length() > 0){
        natureOfLink = data[10];
    }

}
void cLink::attachScene(cScene* scene){
 QGraphicsLineItem * line;
 line = new QGraphicsLineItem(QLineF(gPersonFrom->TopHook(),gPersonTo->BottomHook()));
 scene->addItem(line);
}

void cLink::attachPersons(gPerson * person1, gPerson * person2){
 gPersonFrom = person1;
 gPersonTo = person2;
}
