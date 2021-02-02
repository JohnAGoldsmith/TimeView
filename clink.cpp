#include "clink.h"

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
