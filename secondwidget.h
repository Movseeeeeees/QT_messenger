#ifndef SECONDWIDGET_H
#define SECONDWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QGridLayout>
#include <QLabel>
#include <QPixmap>

class SecondWidget : public QWidget
{
    Q_OBJECT

public:
    SecondWidget(QWidget *parent = nullptr);
    ~SecondWidget();
private:
    QLineEdit* name;
    QLineEdit* surname;
    QLineEdit* mail;
    QLineEdit* phone;
    QPixmap* pixmap;
    QLabel* label_png;

    QPushButton* create;
    QPushButton* load;

};


#endif // SECONDWIDGET_H
