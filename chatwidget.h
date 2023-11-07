#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QGroupBox>
#include <QTextEdit>


class chatwidget : public QWidget {
    Q_OBJECT

public:
    chatwidget(QWidget *parent = nullptr);

private:
    QGroupBox* activ_users;
    QGroupBox* communicate;
    QTextEdit* text;
    QLineEdit* message;

    QPushButton* send;
    QPushButton* logout;

signals:
         // Define any custom signals here

private slots:
               // Define any custom slots here
};

#endif // MYWIDGET_H
