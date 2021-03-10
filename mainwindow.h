#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QTableWidget>
#include "cscene.h"
#include "data.h"
#include "clineedit.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    cData* getData(){return &Data;}

private:
    Ui::MainWindow *ui;
    cScene *scene;
    QGraphicsView *view;
    cLineEdit * myLineEdit;
    QTableWidget *personTable;
    QTableWidget *linkTable;
    cData Data;

    void StartAfresh();
    void keyPressEvent(QKeyEvent *);



};
#endif // MAINWINDOW_H
