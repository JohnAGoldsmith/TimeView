#include <QBoxLayout>
#include <QGraphicsView>
#include <QRect>
#include <QJsonObject>
#include <QGraphicsRectItem>
#include <QGraphicsScale>
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


    QHBoxLayout *layout = new QHBoxLayout;

    cScene *  localScene = new cScene();
    cView * view = new cView(localScene);
    view->scale(1,1);
    view->centerOn(-100,1900);
    layout->addWidget(view);

    QWidget *widget = new QWidget;
    widget->setLayout(layout);

    //getData()->ReadJson( );
    getData()->ReadCSV();
    getData()->analyzeData();
    getData()->sendPersonsAndLinksToScene(localScene);

    setCentralWidget(widget);
    setWindowTitle(tr("Genealogy"));
    setUnifiedTitleAndToolBarOnMac(true);

    QJsonObject dataObject;
    getData()->save( );

}

MainWindow::~MainWindow()
{
    delete ui;
}





