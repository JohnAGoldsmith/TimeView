#ifndef HELPWIDGET_H
#define HELPWIDGET_H

#include <QWidget>
#include <QTableWidget>

class HelpWidget : public QWidget
{  Q_OBJECT
public:
    explicit HelpWidget(int rows, int cols, QObject * parent = nullptr);
    ~HelpWidget();

    // QPaintDevice interface
public:
   // QPaintEngine *paintEngine() const;


private:
    QTableWidget * helptable;
};

#endif // HELPWIDGET_H
