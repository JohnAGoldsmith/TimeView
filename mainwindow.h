#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "cscene.h"
#include "data.h"

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

    cData Data;
};
#endif // MAINWINDOW_H
