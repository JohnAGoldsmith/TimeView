#include <QWidget>
#include <QVBoxLayout>
#include <QHeaderView>
#include "helpwidget.h"

cHelpWidget::cHelpWidget(int rows, int cols, QObject * object)
{
    setMinimumSize(500,400);
    setWindowTitle("Shortcuts");
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

    newItem = new QTableWidgetItem("CTRL Z" );
    helptable->setItem(4,0, newItem);
    newItem = new QTableWidgetItem("Clear all data." );
    helptable->setItem(4,1, newItem);

    newItem = new QTableWidgetItem("2 and 8" );
    helptable->setItem(5,0, newItem);
    newItem = new QTableWidgetItem("Shrink or expand vertically." );
    helptable->setItem(5,1, newItem);

    newItem = new QTableWidgetItem("4 and 6" );
    helptable->setItem(6,0, newItem);
    newItem = new QTableWidgetItem("Shrink or expand horizontally." );
    helptable->setItem(6,1, newItem);



    newItem = new QTableWidgetItem("CTRL N" );
    helptable->setItem(7,0, newItem);
    newItem = new QTableWidgetItem("Create new person." );
    helptable->setItem(7,1, newItem);

    newItem = new QTableWidgetItem(" " );
    helptable->setItem(8,0, newItem);
    newItem = new QTableWidgetItem("Selected person:" );
    helptable->setItem(8,1, newItem);

    newItem = new QTableWidgetItem("CTRL B" );
    helptable->setItem(9,0, newItem);
    newItem = new QTableWidgetItem("Move selected person to correct height, given birth" );
    helptable->setItem(9,1, newItem);

    newItem = new QTableWidgetItem(" 4 <-> 8 and 2 <-> 8 " );
    helptable->setItem(10,0, newItem);
    newItem = new QTableWidgetItem("Keep selected item centered in view" );
    helptable->setItem(10,1, newItem);

}
cHelpWidget::~cHelpWidget(){



}
