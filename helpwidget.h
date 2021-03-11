#ifndef HELPWIDGET_H
#define HELPWIDGET_H

#include <QWidget>
#include <QTableWidget>

class cHelpWidget : public QWidget
{  Q_OBJECT
public:
    explicit cHelpWidget(int rows, int cols, QObject * parent = nullptr);
    ~cHelpWidget();

    // QPaintDevice interface
public:
   // QPaintEngine *paintEngine() const;


private:
    QTableWidget * helptable;
};

#endif // HELPWIDGET_H
