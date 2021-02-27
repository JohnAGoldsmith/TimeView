#include <QBoxLayout>
#include <QGraphicsView>
#include <QRect>
#include <QJsonObject>
#include <QGraphicsRectItem>
#include <QGraphicsScale>
#include <QKeyEvent>
#include <QTableWidget>
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

    cScene *  localScene = new cScene();
    cView * view = new cView(localScene);
    view->scale(1,1);
    view->centerOn(-100,1900);
    layout->addWidget(view);

    QWidget *widget = new QWidget;
    widget->setLayout(layout);

    bool Json(true);
    if (Json){
        getData()->A_ReadJson( );
        getData()->A_sendPersonsAndLinksToSceneJson(localScene);
        setCentralWidget(widget);
        setWindowTitle(tr("Genealogy"));
        setUnifiedTitleAndToolBarOnMac(true);

        QJsonObject dataObject;
        getData()->save( );

    }else{
        getData()->A_ReadCSV();
        getData()->A_analyzeData();
        getData()->A_sendPersonsAndLinksToScene(localScene);
        setCentralWidget(widget);
        setWindowTitle(tr("Genealogy"));
        setUnifiedTitleAndToolBarOnMac(true);



    }

    QJsonObject dataObject;
    getData()->save( );



}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::keyPressEvent(QKeyEvent * event){
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
  if (event->key() == Qt::Key_S && event->modifiers()==Qt::CTRL){
      getData()->save( );
  }
  QMainWindow::keyPressEvent(event);
}




