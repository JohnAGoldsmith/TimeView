#include <QLineEdit>
#include <QVBoxLayout>
#include <QLabel>
#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include "cpersonwidget.h"
#include "cscene.h"
#include "mainwindow.h"
#include "columnarscene.h"

cPersonWidget::cPersonWidget(MainWindow * window)
{
    mainwindow = window;

    setMinimumSize(500,400);
    setWindowTitle("New person");
    QVBoxLayout * layout = new QVBoxLayout(this);

    QHBoxLayout * hlayout = new QHBoxLayout();
    QLabel * label = new QLabel(QObject::tr("First name:"));
    firstNameEdit = new QLineEdit();
    hlayout->addWidget(label);
    hlayout->addWidget(firstNameEdit);
    layout->addLayout(hlayout);

    hlayout = new QHBoxLayout();
    label = new QLabel(QObject::tr("Last name:"));
    lastNameEdit = new QLineEdit();
    hlayout->addWidget(label);
    hlayout->addWidget(lastNameEdit);
    layout->addLayout(hlayout);

    hlayout = new QHBoxLayout();
    label = new QLabel(QObject::tr("Born"));
    birthEdit = new QLineEdit();
    hlayout->addWidget(label);
    hlayout->addWidget(birthEdit);
    layout->addLayout(hlayout);

    hlayout = new QHBoxLayout();
    label = new QLabel(QObject::tr("Died:"));
    deathEdit = new QLineEdit();
    hlayout->addWidget(label);
    hlayout->addWidget(deathEdit);
    layout->addLayout(hlayout);

    QPushButton * button = new QPushButton();
    button->setText("Create person.");
    layout->addWidget(button);

    setLayout(layout);



    show();

    connect(button, &QPushButton::clicked,[=](){createPerson(window);} );
}

void  cPersonWidget::createPerson(MainWindow *  mainwindow){

    gPerson * person;
    QStringList info;
    info.append(QString());
    info.append(firstNameEdit->text());
    info.append(lastNameEdit->text());
    info.append(birthEdit->text());
    info.append(deathEdit->text());
    info.append(""); // x from spreadsheet
    info.append(""); //  profession
    person = new gPerson(info);
    if (mainwindow->Scene() ) {
        mainwindow->Scene()->addItem(person);
        person->Scene(mainwindow->Scene() );
        person->setPos(0.0, 0.0);
        person->rememberPos(QPointF(0.0, 0.0));

    }else if (mainwindow->ColScene() ) {
        mainwindow->ColScene()->addItem(person);
        person->Scene(mainwindow->ColScene() );
        person->setPos(0.0,0.0);
        person->rememberPos(QPointF(0.0, 0.0));
    }
    mainwindow->Scene()->update();




}
