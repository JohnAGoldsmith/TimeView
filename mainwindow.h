#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QTableWidget>
#include <QGraphicsView>
#include <QSettings>
#include "cscene.h"
#include "data.h"
#include "clineedit.h"
#include "helpwidget.h"
#include "cpersonwidget.h"
#include "userrequestdialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QSize sizeHint() const;
    cData* getData(){return &Data;}
    cScene* Scene() {return scene;}
    columnarScene * ColScene() {return colScene;}

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
    userRequestDialog * userrequestdialog;
    QSettings settings;



    void createActions();
    void StartAfresh();
    void makeColumnarScene();
    void keyPressEvent(QKeyEvent *);
    QSize sizeHint();
    void openPersonsTable();


};
#endif // MAINWINDOW_H
