#include <QBoxLayout>
#include <QGraphicsView>
#include <QRect>
#include <QJsonObject>
#include <QGraphicsRectItem>
#include <QGraphicsScale>
#include <QKeyEvent>
#include <QTableWidget>
#include <QFileDialog>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cscene.h"
#include "dataperson.h"
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

    QHBoxLayout *layout = new QHBoxLayout;

    scene = new cScene();
    view = new cView(scene);
    view->scale(1,1);
    view->centerOn(-100,1900);
    layout->addWidget(view);

    QWidget *widget = new QWidget;
    widget->setLayout(layout);


    setCentralWidget(widget);
    setWindowTitle(tr("Genealogy"));
    setUnifiedTitleAndToolBarOnMac(true);

    bool Json(true);
    if (Json){
        QString jsonfilename = "./timeview.json";
        getData()->A_ReadJson(jsonfilename);
        getData()->A_sendPersonsAndLinksToSceneJson(scene);
    }else{
        QString csvfilename = "./timeview.csv";
        getData()->A_ReadCSV(csvfilename);
        getData()->A_analyzeData();
        getData()->A_sendPersonsAndLinksToScene(scene);
    }
    //QJsonObject dataObject;
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
void MainWindow::keyPressEvent(QKeyEvent * event){                         /* Don't forget "focusItem" */
    if (event->key() == Qt::Key_C && event->modifiers()==Qt::CTRL){
            getData()->MoveInvisibleToLimbo();         
    }

  if (event->key() == Qt::Key_D && event->modifiers()==Qt::CTRL){
      if (! personTable){
          personTable  = new QTableWidget;
          getData()->populatePersonTable(personTable);
          personTable->show();
      }
  }

  if (event->key() == Qt::Key_L&& event->modifiers()==Qt::CTRL){
      if (! linkTable){
          linkTable  = new QTableWidget;
          getData()->populateLinkTable(linkTable);
          linkTable->show();
      }
  }

  /*         Open                                    */
  if (event->key() == Qt::Key_O && event->modifiers()==Qt::CTRL){
       QString fileName = QFileDialog::getOpenFileName(this,"Open document",QDir::currentPath(), "*.csv *.json");
       if (!fileName.isEmpty()){
          if (fileName.endsWith(".json")){
              getData()->A_ReadJson(fileName);
              getData()->A_sendPersonsAndLinksToSceneJson(scene);
          } else if (fileName.endsWith(".csv")){
              getData()->A_ReadCSV(fileName);
              getData()->A_analyzeData();
              getData()->A_sendPersonsAndLinksToScene(scene);
          }
       }
  }

  if (event->key() == Qt::Key_S && event->modifiers()==Qt::CTRL){
      getData()->save( );
  }

  if (event->key() == Qt::Key_X && event->modifiers()==Qt::CTRL){
      getData()->save( );
      qApp->exit( );
  }
  /*          Clear all data                 */
  if (event->key() == Qt::Key_Z && event->modifiers()==Qt::CTRL){
    getData()->Clear();
    scene->clear();

  }
  QMainWindow::keyPressEvent(event);
}




