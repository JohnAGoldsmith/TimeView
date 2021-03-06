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
#include <QFontDialog>
#include <QListWidget>
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
    void readLinks( QList<cLink*>* );

private:
    cData* Data;
    cScene * Scene;
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
    QFontDialog * fontDialog;

    QListWidget * linkList;
    QListWidget * colorList;
    cLink * myLink;

    QSpinBox  * groupheightSpin;
    QSpinBox * groupwidthSpin;
    QLineEdit * groupNameLE;
    QPushButton * addGroupbutton;
    QPushButton * modifyGroupbutton;
    cGroup * latestGroup;
    void addNewGroup();
    void changeGroupSize();
    void keyPressEvent(QKeyEvent *);



public slots:
    void openFontDialog();
    void grabLink(QListWidgetItem * listwidgetitem);
    void setLinkColor(QListWidgetItem * colorItem);
};

#endif // USERREQUESTDIALOG_H
