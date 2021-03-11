#include <QWidget>
#include <QVBoxLayout>
#include <QHeaderView>
#include "helpwidget.h"

HelpWidget::HelpWidget(int rows, int cols, QObject * object)
{
    setMinimumSize(500,200);
    QVBoxLayout * layout = new QVBoxLayout(this);
    helptable = new QTableWidget(rows, cols);
    layout->addWidget(helptable);
    setLayout(layout);
    QStringList labels;
    helptable->horizontalHeader()->hide();
    helptable->verticalHeader()->hide();
    //helptable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    helptable->setColumnWidth(0,40);
    helptable->setColumnWidth(1,400);

    QTableWidgetItem *newItem = new QTableWidgetItem("CTRL L" );
    helptable->setItem(0,0, newItem);
    newItem = new QTableWidgetItem("Show Links table" );
    helptable->setItem(0,1, newItem);

    newItem = new QTableWidgetItem("CTRL D" );
    helptable->setItem(1,0, newItem);
    newItem = new QTableWidgetItem("Show Persons table" );
    helptable->setItem(1,1, newItem);

    newItem = new QTableWidgetItem("CTRL H" );
    helptable->setItem(2,0, newItem);
    newItem = new QTableWidgetItem("Make grayed persons invisible" );
    helptable->setItem(2,1, newItem);

    newItem = new QTableWidgetItem("CTRL I" );
    helptable->setItem(3,0, newItem);
    newItem = new QTableWidgetItem("Make invisible persons visible but grayed" );
    helptable->setItem(3,1, newItem);
}
HelpWidget::~HelpWidget(){



}
