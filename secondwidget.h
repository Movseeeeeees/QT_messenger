#ifndef SECONDWIDGET_H
#define SECONDWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QGridLayout>
#include <QLabel>
#include <QPixmap>
#include <QSqlDatabase>

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
    QLineEdit* password;
    QLineEdit* password_check;
    QPixmap* pixmap;
    QLabel* label_png;
    QPushButton* create;
    QPushButton* load;

    QSqlDatabase db;

    QByteArray fdtos;
    QString imagename;
    QString hashing(const QString &password);
    void insert_data(QString name,QString surname,QString mail,QString phone ,QString password ,QString imagename, QByteArray fdtos);

private slots:
    void create_account();
    void load_image();
};


#endif // SECONDWIDGET_H
