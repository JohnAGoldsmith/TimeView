#include <QDebug>
#include "dataperson.h"

struct Path{

};



dPerson::dPerson()
{

}


dPerson::dPerson(QStringList data){
    key = "no_key";

    if (data.size() >= 8 && data[7].length() > 0){
        key = data[7];
    } else {
        key = data[2];
    }
    firstName = data[1];
    lastName = data[2];
    birthYear = data[3].toInt();
    if (birthYear == 0){
        qDebug() << lastName << "zero birth year a";
    }
    if (data[4].length() > 0){
        deathYear = data[4].toInt();
    } else {
        deathYear = 0;
        qDebug() << lastName << "Zero death year" ;
    }
    xpos = data[5].toFloat();
    profession1 = data[6];
    graphicPerson = NULL;

}

void dPerson::write(QJsonObject & json) const {
    json["key"] = key;
    json["type"] = "person";
    json["firstName"] = firstName;
    json["lastName"] = lastName;
    json["birthYear"] = birthYear;
    json["deathYear"] = deathYear;
    json["xpos"] = xpos;
    json["profession1"] = profession1;

}

