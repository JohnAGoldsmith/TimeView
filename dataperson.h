#ifndef DATAPERSON_H
#define DATAPERSON_H
#include <QStringList>

class cdataPerson;
class Path;

class cdataPerson
{
    friend class cData;
public:
  cdataPerson(QStringList) ;
  cdataPerson();

private:
  QString firstName;
  QString lastName;
  QString key;
  int birthYear;
  int deathYear;
  int xpos;
  int ypos;
  QString profession1;
  QString profession2;
};




#endif // DATAPERSON_H
