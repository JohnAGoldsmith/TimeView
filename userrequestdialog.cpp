#include "userrequestdialog.h"
#include "ui_userrequestdialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QFontDialog>

userRequestDialog::userRequestDialog(cData * data,  cScene * scene, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::userRequestDialog)
{

    ui->setupUi(this);
    mytabWidget = new QTabWidget(this);

    Data = data;
    Scene = scene;

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
    connect(changeFontButton,SIGNAL(clicked( )),this,SLOT(openFontDialog()));


    QPushButton * addPersonbutton = new QPushButton("Add new person",page1);
    vlayout1->addWidget(addPersonbutton);

    page1->setLayout(vlayout1);
    mytabWidget->addTab(page1, "Add person");


    /* Page 2                 Lines                     */
    QWidget * page2 = new QWidget(mytabWidget);
    vlayout1 = new QVBoxLayout();

    hlayout = new QHBoxLayout();
    hlayout->addWidget(new QLabel("Line width",page2));
    lineWidthSpin = new QSpinBox(page2);
    hlayout->addWidget(lineWidthSpin);
    vlayout1->addLayout(hlayout);

    hlayout = new QHBoxLayout();
    vlayout1->addLayout(hlayout);
    page2->setLayout(vlayout1);
    QSpacerItem *  verticalSpacer = new QSpacerItem(20, 40);
    vlayout1->addItem(verticalSpacer);
    mytabWidget->addTab(page2, "Lines");


    /* Page 3                  Groups                  */



    QWidget * page3 = new QWidget(mytabWidget);
    vlayout1 = new QVBoxLayout(page3);

    hlayout = new QHBoxLayout();
    addGroupbutton = new QPushButton("Create group");
    hlayout->addWidget(addGroupbutton);
    vlayout1->addLayout(hlayout);

    hlayout = new QHBoxLayout();
    hlayout->addWidget(new QLabel("Height"));
    groupheightSpin = new QSpinBox();
    groupheightSpin->setRange(100,2000);
    groupheightSpin->setSingleStep(100);

    hlayout->addWidget(groupheightSpin);
    vlayout1->addLayout(hlayout);

    hlayout = new QHBoxLayout();
    hlayout->addWidget(new QLabel("Width"));
    groupwidthSpin = new QSpinBox();
    groupwidthSpin->setRange(100,2000);
    groupwidthSpin->setSingleStep(100);
    hlayout->addWidget(groupwidthSpin);
    vlayout1->addLayout(hlayout);

    hlayout = new QHBoxLayout();
    hlayout->addWidget(new QLabel("Name"));
    groupNameLE = new QLineEdit();
    hlayout->addWidget(groupNameLE);
    vlayout1->addLayout(hlayout);

    page3->setLayout(vlayout1);
    mytabWidget->addTab(page3,"Groups");
    connect(addGroupbutton, &QPushButton::clicked, this,&userRequestDialog::addNewGroup);

    mytabWidget->show();





}

void userRequestDialog::addNewGroup(){

    QStringList data;
    float x  = 0;
    float y = 1900;
    int height = groupheightSpin->value();
    int width = groupwidthSpin->value();



    QString name = groupNameLE->text();
    QString key = name;
    cGroup * group = new cGroup (key, name, x,y,height,width);
    if (! Data->validateNewGroup(group)){ //validate should include sending to collections
        delete group;
        /* send message of failure.  */
    }
    group->setY ( Scene->ConvertYearToYcoor( group->Y() ) );
    Scene->addItem(group);
    group->setPos(group->X(), group->Y());
    qDebug() << "data group line 200" << group->X() << group->Y();

}



userRequestDialog::~userRequestDialog()
{
    delete ui;
}

void userRequestDialog::openFontDialog()
{
    fontDialog = new QFontDialog(this);
    fontDialog->show();
}
void userRequestDialog::keyPressEvent(QKeyEvent * event){
    if (event->key() == Qt::Key_X && event->modifiers()==Qt::CTRL){
          close( );
    }
}