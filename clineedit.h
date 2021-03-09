#ifndef CLINEEDIT_H
#define CLINEEDIT_H

#include <QLineEdit>

class cLineEdit : public QLineEdit
{
public:
    cLineEdit();
public slots:
    void setLineEditText(QString);
};

#endif // CLINEEDIT_H
