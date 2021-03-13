#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QTableWidget>
#include <QGraphicsView>
#include "cscene.h"
#include "data.h"
#include "clineedit.h"
#include "helpwidget.h"
#include "cpersonwidget.h"

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
    columnarScene * colScene;
    QGraphicsView *view;
    cLineEdit * myLineEdit;
    QTableWidget *personTable;
    QTableWidget *linkTable;
    cData Data;
    cHelpWidget * helpwidget;
    cPersonWidget * newpersonwidget;


    void StartAfresh();
    void makeColumnarScene();
    void keyPressEvent(QKeyEvent *);




};
#endif // MAINWINDOW_H
