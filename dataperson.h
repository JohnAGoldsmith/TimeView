#ifndef DATAPERSON_H
#define DATAPERSON_H
#include <QStringList>
#include <QJsonObject>

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

  void write(QJsonObject & json) const;

  QString Key() {return key;}
  QString FirstName() {return firstName;}
  QString LastName() {return lastName;}
  float Xpos() {return xpos;}
  void Ypos(float yvalue) {ypos = yvalue;}
  float Ypos() {return ypos;}
  int BirthYear() const {return birthYear;}
  int DeathYear() const {return deathYear;}
  gPerson* get_gPerson() {return graphicPerson;}
  void set_gPerson(gPerson* gp){graphicPerson = gp;}


private:
  QString firstName;
  QString lastName;
  QString key;
  int birthYear;
  int deathYear;
  float xpos;
  float ypos;
  QString profession1;
  QString profession2;
  gPerson* graphicPerson;

};




#endif // DATAPERSON_H
