#include <QBoxLayout>
#include <QGraphicsView>
#include <QRect>
#include <QJsonObject>
#include <QGraphicsRectItem>
#include <QGraphicsScale>
#include <QKeyEvent>
#include <QTableWidget>
#include <QFileDialog>
#include <QLineEdit>
#include <QLabel>
#include <QVBoxLayout>
#include <QSlider>



#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cscene.h"
#include "gperson.h"
#include "clink.h"
#include "data.h"
#include "cview.h"




MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    personTable = NULL;
    linkTable = NULL;
    newpersonwidget = NULL;
    userrequestdialog = NULL;

    QVBoxLayout *layout = new QVBoxLayout;
    helpwidget = new cHelpWidget(12,2,this);
    helpwidget->setAttribute(Qt::WA_DeleteOnClose);
    helpwidget->show();

    scene = new cScene();
    view = new cView(scene);
    view->scale(1,1);
    view->centerOn(-100,1900);
    layout->addWidget(view);
    colScene  = nullptr;

    QSettings settings ("JohnAGoldsmith", "TimeView");
    //QString colorfolder = settings.value("colorfolder");
    //QString datafolder = settings.value("datafolder");


    QWidget *widget = new QWidget;
    widget->setLayout(layout);
    myLineEdit = new cLineEdit();
    layout->addWidget(myLineEdit);
    QSlider * myHorizontalSlider = new QSlider(Qt::Horizontal, this);
    myHorizontalSlider->setRange(-10,10);
    myHorizontalSlider->setValue(0);
    layout->addWidget(myHorizontalSlider);

    setCentralWidget(widget);
    setWindowTitle(tr("Genealogy"));
    setUnifiedTitleAndToolBarOnMac(true);

    createActions();

    QString LocationOfData = QDir::homePath() + "/Dropbox/TimeView/data";
    QString fileName = QFileDialog::getOpenFileName(this,"Open document",LocationOfData, "*.csv *.json");

    if (!fileName.isEmpty()){
       setWindowTitle(fileName);
       if (fileName.endsWith(".json")){
           getData()->A_ReadJson(fileName);
           getData()->A_sendPersonsAndLinksToSceneJson(scene);
       } else if (fileName.endsWith(".csv")){
           if (fileName.endsWith("legacy.csv")){
               getData()->A_ReadCSV(fileName);
               getData()->A_analyzeLegacyCSVdata();
               getData()->A_sendPersonsAndLinksToScene(scene);
           } else{
               getData()->A_ReadCSV(fileName);
               getData()->A_analyzeCSVdata();
               getData()->A_sendPersonsAndLinksToScene(scene);
           }
       }
    }

    connect(myHorizontalSlider, SIGNAL(valueChanged(int)), scene, SLOT(changeHorizontalScale(int)));

}
void MainWindow::createActions(){
/*
    QAction * saveAction = new QAction(tr("&Save"),this);
    saveAction->setShortcut(Qt::CTRL + Qt::Key_S);
    QFileDialog *  filedialog = new QFileDialog();
    filedialog->setViewMode(QFileDialog::Detail);
    QString fileName = filedialog->getSaveFileName(this,"Save data",QDir::currentPath(), "*.csv *.json");
    getData()->save(fileName);
    connect(saveAction,&QAction::triggered,&Data,cData::save(fileName));

    QAction * showPersonsTable = new QAction(tr("&Persons"),this);
    showPersonsTable->setShortcut(Qt::CTRL + Qt::Key_D);
    connect(showPersonsTable, &QAction::triggered, this,&MainWindow::openPersonsTable);
*/
}

void MainWindow::openPersonsTable(){
    if (! personTable){
        personTable  = new QTableWidget;
        getData()->populatePersonTable(personTable);
        personTable->show();
        myLineEdit->setText("Load persons table.");
    }
}
void MainWindow::makeColumnarScene(){
    if (! colScene){
        colScene = new columnarScene();
        delete scene;
        scene = NULL;
    }
    getData()->sendPersonsToColumnarScene(colScene);
    view->setScene(colScene);
}

void MainWindow::StartAfresh(){
    if (personTable){
        delete personTable;
        personTable = NULL;
    }
    if (linkTable){
        delete linkTable;
        linkTable = NULL;
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::keyPressEvent(QKeyEvent * event){


    /*            Add text to line-edit                */
    myLineEdit->setText(myLineEdit->text() + event->text());

    /*          Make grayed boxes invisible            */
    if (event->key() == Qt::Key_H && event->modifiers()==Qt::CTRL){
         getData()->GrayedPersons2Invisible();
    }

    /*             Make invisible boxes visible but grayed  */
    else if (event->key() == Qt::Key_I && event->modifiers()==Qt::CTRL){
         getData()->InvisiblePersons2Grayed();
    }

    /* Create a columnar scene */
    if (event->key() == Qt::Key_C && event->modifiers()==Qt::CTRL){
        makeColumnarScene();
    }

  /*                Load table of persons for user              */

    if (event->key() == Qt::Key_D && event->modifiers()==Qt::CTRL){
        if (! personTable){
            personTable  = new QTableWidget;
            getData()->populatePersonTable(personTable);
            personTable->show();
            myLineEdit->setText("Load persons table.");
        }
    }


  /*           Load table of links for user                      */
  if (event->key() == Qt::Key_L&& event->modifiers()==Qt::CTRL){
      if (! linkTable){
          linkTable  = new QTableWidget;
          getData()->populateLinkTable(linkTable);
          linkTable->show();
          myLineEdit->setText("Load link table.");
      }
  }

  /*         Open  file                                */
  if (event->key() == Qt::Key_O && event->modifiers()==Qt::CTRL){
       QString LocationOfData = QDir::homePath() + "/Dropbox/TimeView/data";
       QString fileName = QFileDialog::getOpenFileName(this,"Open document",LocationOfData, "*.csv *.json");
       if (!fileName.isEmpty()){
          setWindowTitle(fileName);
          if (fileName.endsWith(".json")){
              getData()->A_ReadJson(fileName);
              getData()->A_sendPersonsAndLinksToSceneJson(scene);
              myLineEdit->setText("Open json file.");
          } else if (fileName.endsWith(".csv")){
              if (fileName.endsWith("legacy.csv")){
                  getData()->A_ReadCSV(fileName);
                  getData()->A_analyzeLegacyCSVdata();
                  getData()->A_sendPersonsAndLinksToScene(scene);
                  myLineEdit->setText("Open legacy csv file.");
              } else{
                  getData()->A_ReadCSV(fileName);
                  getData()->A_analyzeCSVdata();
                  getData()->A_sendPersonsAndLinksToScene(scene);
                  myLineEdit->setText("Open csv file.");
              }
          }
       }
  }

  /*                 Save                       */

  if (event->key() == Qt::Key_S && event->modifiers()==Qt::CTRL){
      QFileDialog *  filedialog = new QFileDialog();
      filedialog->setViewMode(QFileDialog::Detail);
      QString fileName = filedialog->getSaveFileName(this,"Save data",QDir::currentPath(), "*.csv *.json");
      getData()->save(fileName);
      myLineEdit->setText("Save data to Json and csv files.");
  }

  /*        exit program          */
  if (event->key() == Qt::Key_X && event->modifiers()==Qt::CTRL){
      if (helpwidget)
          helpwidget->close();
      if (newpersonwidget)
          newpersonwidget->close();
      if (userrequestdialog)
          userrequestdialog->close();
      if (linkTable)
          linkTable->close();
      if (personTable)
          personTable->close();
      //settings.setValue("colorfolder", QDir::homePath() + "/Dropbox/TimeView/color";)
      //settings.setValue("datafolder", QDir::homePath() + "/Dropbox/TimeView/data";)
      qApp->exit( );
  }

  /*          Clear all data                 */
  if (event->key() == Qt::Key_Z && event->modifiers()==Qt::CTRL){
      getData()->Clear();
      scene->clear();
      scene->update();
      myLineEdit->setText("Clear all data.");
  }

  /*          Open widget to add new person                 */
  else if (event->key() == Qt::Key_N && event->modifiers()==Qt::CTRL){
      /*
      if (newpersonwidget){
          delete newpersonwidget;}
      newpersonwidget = new cPersonWidget(this);
      newpersonwidget->show();
      myLineEdit->setText("Create new person.");
      */
      if (userrequestdialog){
          delete userrequestdialog;
      }
      //qDebug() << "Mainwindow making new dialg";
      userrequestdialog = new userRequestDialog(getData(), Scene(),this);
      userrequestdialog->show();
  }

  QMainWindow::keyPressEvent(event);

}

QSize MainWindow::sizeHint() const
{
    return QSize (1800,2000);
}



