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
    linkList = new QListWidget(this);
    hlayout->addWidget(linkList);
    vlayout1->addItem(hlayout);

    hlayout = new QHBoxLayout();
    hlayout->addWidget(new QLabel("Line width",page2));
    lineWidthSpin = new QSpinBox(page2);
    hlayout->addWidget(lineWidthSpin);
    vlayout1->addLayout(hlayout);

    hlayout = new QHBoxLayout();
    colorList = new QListWidget(this);
    foreach (QString colorname, QColor::colorNames()){
        new QListWidgetItem(colorname,colorList);
    }
    hlayout->addWidget(colorList);
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
    groupheightSpin->setValue(1000);

    hlayout->addWidget(groupheightSpin);
    vlayout1->addLayout(hlayout);

    hlayout = new QHBoxLayout();
    hlayout->addWidget(new QLabel("Width"));
    groupwidthSpin = new QSpinBox();
    groupwidthSpin->setRange(100,2000);
    groupwidthSpin->setSingleStep(100);
    groupwidthSpin->setValue(1000);
    hlayout->addWidget(groupwidthSpin);
    vlayout1->addLayout(hlayout);

    hlayout = new QHBoxLayout();
    hlayout->addWidget(new QLabel("Name"));
    groupNameLE = new QLineEdit();
    hlayout->addWidget(groupNameLE);
    vlayout1->addLayout(hlayout);

    hlayout = new QHBoxLayout();
    modifyGroupbutton = new QPushButton("Modify group size");
    hlayout->addWidget(modifyGroupbutton);
    vlayout1->addLayout(hlayout);


    page3->setLayout(vlayout1);
    mytabWidget->addTab(page3,"Groups");




    connect(addGroupbutton, &QPushButton::clicked, this,&userRequestDialog::addNewGroup);
    connect(modifyGroupbutton,&QPushButton::clicked,
            this, &userRequestDialog::changeGroupSize );
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
    latestGroup = new cGroup (key, name, x,y,height,width);
    if (! Data->validateNewGroup(latestGroup)){ //validate should include sending to collections
        delete latestGroup;
        return;
        /* send message of failure.  */
    }
    latestGroup->setY ( Scene->ConvertYearToYcoor( latestGroup->Y() ) );
    Scene->addItem(latestGroup);
    latestGroup->setPos(latestGroup->X(), latestGroup->Y());
    //qDebug() << "data group line 200" << latestGroup->X() << latestGroup->Y();

}
void userRequestDialog::changeGroupSize(){
    if (latestGroup){
        latestGroup->changeWidth(groupwidthSpin->value());
        latestGroup->changeHeight(groupheightSpin->value());
    }
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
void userRequestDialog::readLinks(QList<cLink *> * Links){
    linkList->setSortingEnabled(true);
    foreach (cLink* link, *Links ){
       new QListWidgetItem(link->getKey(),linkList);
    }

    linkList->update();
}
