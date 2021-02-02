#include <QBoxLayout>
#include <QGraphicsView>
#include <QRect>

#include <QGraphicsRectItem>
#include <QGraphicsScale>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cscene.h"
#include "dataperson.h"
#include "gperson.h"
#include "clink.h"
#include "data.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    QHBoxLayout *layout = new QHBoxLayout;

    cScene *  localScene = new cScene();
    QGraphicsView * view = new QGraphicsView(localScene);
    view->scale(2,2);
    view->centerOn(0,1900);
    layout->addWidget(view);

    QWidget *widget = new QWidget;
    widget->setLayout(layout);

    getData()->ReadCSV();
    getData()->analyzeData();
    getData()->sendPersonsToScene(localScene);

    setCentralWidget(widget);
    setWindowTitle(tr("Genealogy"));
    setUnifiedTitleAndToolBarOnMac(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}





