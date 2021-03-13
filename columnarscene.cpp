#include "columnarscene.h"
#include <algorithm>
#include <vector>
#include "gperson.h"
#include "cview.h"

class gPerson;

columnarScene::columnarScene()
{

}

void columnarScene::addToColumnar(gPerson * person){
    columnar.append(person);
}

bool bornAfter (gPerson * person1, gPerson* person2){
    return person1->BirthYear() > person2->BirthYear();
}
void columnarScene::sortColumnar(){
   std::sort(columnar.begin(),columnar.end(),bornAfter);
   foreach (gPerson* person, columnar){
       qDebug() << person->LastName() << person->BirthYear();
   }
}
void columnarScene::displayColumnar(){
    float currentY;
    if (columnar.size()){
        currentY = columnar[0]->BirthYear();
    } else{
        return;
    }
    foreach (gPerson* person, columnar){
        addItem(person);
        person->Scene(this); // why is this necessary? Why can't I get this from the gperson?
        person->setPos(0.0, currentY );
        person->rememberPos(QPoint( person->Xpos(), person->Ypos()) );
        currentY += person->Height();
    }
}
