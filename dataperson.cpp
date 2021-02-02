#include <QDebug>
#include "dataperson.h"

struct Path{

};



dPerson::dPerson()
{

}


dPerson::dPerson(QStringList data){
    if (data.size() >= 8 && data[7].length() > 0){
        key = data[7];
    } else {
        key = data[2];
    }
    firstName = data[1];
    lastName = data[2];
    birthYear = data[3].toInt();
    if (data[4].length() > 0){
        deathYear = data[4].toInt();
    } else {
        deathYear = 0;
    }
    xpos = data[5].toInt();
    profession1 = data[6];
    graphicPerson = NULL;
    qDebug() << "constructor of dataperson" << key;

}



