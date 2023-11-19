#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QHBoxLayout>
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QGroupBox>
#include <QTextBrowser>
#include <QSqlDatabase>
#include <QComboBox>
#include <QMessageBox>
#include <QCloseEvent>


class chatwidget : public QWidget {
    Q_OBJECT

public:
    chatwidget(const QString &user, QWidget *parent = nullptr);

private:
    QGroupBox* activ_users;
    QGroupBox* communicate;
    QGroupBox* logout_box;
    QComboBox* list_users;
    QTextBrowser* text;
    QLineEdit* message;
    QString mail;
    QString to_mail=NULL;
    QPushButton* send;
    QPushButton* logout;
    QSqlDatabase db;
    QVBoxLayout *group_logout;
    QVBoxLayout *group_lay_communicate;
    QVBoxLayout *group_lay_active;
    QVBoxLayout *all;
 struct onlineuserinfo{
        QString name;
        QString surname;
        QString mail;
        QString phone;
        QString image;
        QString imagedata;
        onlineuserinfo(const QString& uname, const QString& uusurname, QString& umail, QString& uphone):name(uname),surname(uusurname),mail(umail),phone(uphone){}
    };
    QMap<QString,onlineuserinfo> map_active_users;
public slots:
    void checkonlineusers();
    void notactive();
    void showactiveusers();
    void connectwithuser();
    void database_init();
    void connects();
    void design();
    void setuser(const QString &user);
    void sendmessage();
    void update_list_messages();
protected:
    void closeEvent(QCloseEvent *event) override
    {
        // Show a confirmation dialog
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Close", "Do you really want to close the application?",
                                      QMessageBox::Yes | QMessageBox::No);

        // Check the user's choice
        if (reply == QMessageBox::Yes) {
            notactive();
            event->accept();
        } else {
            // Ignore the close event
            event->ignore();
        }
    }
signals:
         // Define any custom signals here

private slots:
               // Define any custom slots here
   // void logout();
};

#endif // MYWIDGET_H
