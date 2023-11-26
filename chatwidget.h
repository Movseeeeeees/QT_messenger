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
#include <QThread>
#include <QListWidget>

class chatwidget : public QWidget {
    Q_OBJECT

public:
    chatwidget(const QString &user, QWidget *parent = nullptr);

private:
    QHBoxLayout *l;
    QHBoxLayout *group_lay_active;
    QListWidget *users_list;
    QListWidget *active_users_list;
    QLabel* username;
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
    QHBoxLayout *troup_lay_active;
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
    QMap<QString,onlineuserinfo> map_all_users;
public slots:
    void checkonlineusers();
    void notactive();
    void showactiveusers();
    void connectwithuser();
    void connectwithuser_from_online();
    void database_init();
    void connects();
    void design();
    void setuser(const QString &user);
    void sendmessage();
    void update_list_messages();
    QLabel* get_image_from_db(QString user);
    void get_users_from_db();
    void check_new_message();
    void set_messages_statuse_read(QString user1,QString user2);
protected:
    void closeEvent(QCloseEvent *event) override
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Close", "Do you really want to close the application?",
                                      QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::Yes) {
            notactive();
            event->accept();
        } else {
            event->ignore();
        }
    }
signals:

private slots:
};

#endif // MYWIDGET_H
