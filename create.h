#ifndef NEWWIDGET_H
#define NEWWIDGET_H

#include <QWidget>

class NewWidget : public QWidget
{
    Q_OBJECT

public:
    NewWidget(QWidget *parent = nullptr);
    ~NewWidget();
};

#endif // NEWWIDGET_H

