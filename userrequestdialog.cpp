#include "userrequestdialog.h"
#include "ui_userrequestdialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpacerItem>

userRequestDialog::userRequestDialog(cData *, cScene *, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::userRequestDialog)
{
    ui->setupUi(this);
    mytabWidget = new QTabWidget(this);
    /*  Page 1 Persons              */
    QWidget * page1 = new QWidget(mytabWidget);
    QVBoxLayout * vlayout1;
    vlayout1 = new QVBoxLayout(page1);

    firstNameLE = new QLineEdit(page1);
    lastNameLE = new QLineEdit(page1);
    birthLE = new QLineEdit(page1);
    deathLE = new QLineEdit(page1);
    profession1LE = new QLineEdit(page1);

    QHBoxLayout * hlayout = new QHBoxLayout;
    hlayout->addWidget(new QLabel("First name", page1));
    hlayout->addWidget(firstNameLE);
    vlayout1->addLayout(hlayout);
    hlayout = new QHBoxLayout;
    hlayout->addWidget(new QLabel("Last name", page1));
    hlayout->addWidget(lastNameLE);
    vlayout1->addLayout(hlayout);
    hlayout = new QHBoxLayout;
    hlayout->addWidget(new QLabel("Birth year", page1));
    hlayout->addWidget(birthLE);
    vlayout1->addLayout(hlayout);
    hlayout = new QHBoxLayout;
    hlayout->addWidget(new QLabel("Death year", page1));
    hlayout->addWidget(deathLE);
    vlayout1->addLayout(hlayout);
    hlayout = new QHBoxLayout;
    hlayout->addWidget(new QLabel("Professon 1", page1));
    hlayout->addWidget(profession1LE);
    vlayout1->addLayout(hlayout);

    QPushButton * changeFontButton = new QPushButton("Change font",page1);
    vlayout1->addWidget(changeFontButton);

    QPushButton * addPersonbutton = new QPushButton("Add new person",page1);
    vlayout1->addWidget(addPersonbutton);

    //vlayout1->addSpacerItem (new QSpacerItem(100,100));

    page1->setLayout(vlayout1);
    mytabWidget->addTab(page1, "Add person");


    /* Page 2                 Lines                     */
    QWidget * page2 = new QWidget(mytabWidget);
    vlayout1 = new QVBoxLayout();

    lineWidthSpin = new QSpinBox(page2);

    hlayout = new QHBoxLayout();
    hlayout->addWidget(new QLabel("Line width",page2));
    hlayout->addWidget(lineWidthSpin);
    vlayout1->addLayout(hlayout);
    page2->setLayout(vlayout1);
    mytabWidget->addTab(page2, "Lines");


    /* Page 3                  Groups                  */
    QWidget * page3 = new QWidget(mytabWidget);
    vlayout1 = new QVBoxLayout(page3);
    hlayout = new QHBoxLayout(page3);
    hlayout->addWidget(new QLabel("Create group"));
    vlayout1->addLayout(hlayout);
    page3->setLayout(vlayout1);
    mytabWidget->addTab(page3,"Groups");





    mytabWidget->show();




 ;
}

userRequestDialog::~userRequestDialog()
{
    delete ui;
}
