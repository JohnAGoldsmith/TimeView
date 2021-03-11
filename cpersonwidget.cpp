#include <QLineEdit>
#include <QVBoxLayout>
#include <QLabel>
#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include "cpersonwidget.h"
#include "cscene.h"

cPersonWidget::cPersonWidget()
{
    setMinimumSize(500,400);
    setWindowTitle("New person");
    QVBoxLayout * layout = new QVBoxLayout(this);

    QHBoxLayout * hlayout = new QHBoxLayout();
    QLabel * label = new QLabel(QObject::tr("First name:"));
    QLineEdit * firstName = new QLineEdit();
    hlayout->addWidget(label);
    hlayout->addWidget(firstName);
    layout->addLayout(hlayout);

    hlayout = new QHBoxLayout();
    label = new QLabel(QObject::tr("Last name:"));
    QLineEdit * lastName = new QLineEdit();
    hlayout->addWidget(label);
    hlayout->addWidget(lastName);
    layout->addLayout(hlayout);

    hlayout = new QHBoxLayout();
    label = new QLabel(QObject::tr("Born"));
    QLineEdit * born = new QLineEdit();
    hlayout->addWidget(label);
    hlayout->addWidget(born);
    layout->addLayout(hlayout);

    hlayout = new QHBoxLayout();
    label = new QLabel(QObject::tr("Died:"));
    QLineEdit * died = new QLineEdit();
    hlayout->addWidget(label);
    hlayout->addWidget(died);
    layout->addLayout(hlayout);

    QPushButton * button = new QPushButton();
    button->setText("Create person.");
    layout->addWidget(button);

    setLayout(layout);
    show();

    //connect(button, QPushButton::clicked(),createPerson(scene));
}

gPerson* cPersonWidget::createPerson(cScene *  scene){
    gPerson * person;
    person = new gPerson();
    scene->addItem(person);
}
