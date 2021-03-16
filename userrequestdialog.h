#ifndef USERREQUESTDIALOG_H
#define USERREQUESTDIALOG_H
#include <QWidget>
#include "cscene.h"
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>
#include <QFont>
#include "data.h"


namespace Ui {
class userRequestDialog;
}

class userRequestDialog : public QDialog
{
    Q_OBJECT

public:
    explicit userRequestDialog(cData*, cScene*, QWidget *parent = nullptr);
    ~userRequestDialog();

private:
    QTabWidget * mytabWidget;
    Ui::userRequestDialog *ui;
    QLabel * firstNamelabel;
    QLabel * lastNamelabel;
    QLabel * birthlabel;
    QLabel * deathlabel;
    QLabel * profession1label;
    QLineEdit * firstNameLE;
    QLineEdit * lastNameLE;
    QLineEdit * birthLE;
    QLineEdit * deathLE;
    QLineEdit * profession1LE;
    QPushButton * addPersonbutton;
    QSpinBox * lineWidthSpin;
    QFont * currentFont;

};

#endif // USERREQUESTDIALOG_H
