#ifndef DATAPERSON_H
#define DATAPERSON_H
#include <QStringList>

class gPerson;
//class cPerson;
class Path;

class dPerson
{

    friend class gData;
    friend class gPerson;
public:
  dPerson(QStringList) ;
  dPerson();
  gPerson* GraphicPerson() {return graphicPerson;}
  QString Key() {return key;}
  QString LastName() {return lastName;}
  int Xpos() {return xpos;}
  int BirthYear() {return birthYear;}
  int DeathYear() {return deathYear;}
  gPerson* get_gPerson() {return graphicPerson;}
  void set_gPerson(gPerson* gp){graphicPerson = gp;}

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
  gPerson* graphicPerson;
};




#endif // DATAPERSON_H
