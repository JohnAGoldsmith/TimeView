#ifndef CPERSONWIDGET_H
#define CPERSONWIDGET_H
#include "gperson.h"

#include <QWidget>
class MainWindow;


class cPersonWidget : public QWidget
{
public:
    cPersonWidget(MainWindow* window);
public slots:
    void  createPerson(MainWindow *);
private:
    MainWindow * mainwindow;
    QLineEdit * firstNameEdit;
    QLineEdit * lastNameEdit;
    QLineEdit * birthEdit;
    QLineEdit * deathEdit;
};

#endif // CPERSONWIDGET_H
