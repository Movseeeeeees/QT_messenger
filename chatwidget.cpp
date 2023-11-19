#include <QWidget>
#include "chatwidget.h"
#include "mainwidget.h"
#include <QGridLayout>
#include <QHBoxLayout>
#include <QMenuBar>
#include <QMenu>
#include <QMainWindow>
#include <QAction>
#include <QSqlQuery>
#include <QSqlError>
#include <QLabel>
#include <QDateTime>

chatwidget::chatwidget(const QString &user, QWidget *parent):QWidget(parent){

    setuser(user);

    database_init();

    checkonlineusers();

    design();

    connects();

    showactiveusers();
};
void chatwidget::database_init(){
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("127.0.0.1");
    db.setDatabaseName("my_database");
    db.setUserName("hp");
    db.setPassword("7777777");
    db.setPort(3306);
}
void chatwidget::design(){
    activ_users= new QGroupBox(tr("Active users"));
    communicate= new QGroupBox(tr("Area"));

    message = new QLineEdit;
    message->setPlaceholderText("Message");

    send= new QPushButton(tr("Send"));
    logout = new QPushButton(tr("Log out"));

    QWidget *w=new QWidget;
    QHBoxLayout *hbox=new QHBoxLayout;
    hbox->addWidget(message);
    hbox->addWidget(send);
    w->setLayout(hbox);

    list_users = new QComboBox;

    group_lay_active = new QVBoxLayout;
    group_lay_active->addWidget(list_users);
    group_lay_communicate= new QVBoxLayout;


    text = new QTextBrowser;
    group_lay_communicate->addWidget(text);
    group_lay_communicate->addWidget(w);
    activ_users->setLayout(group_lay_active);
    communicate->setLayout(group_lay_communicate);
    all=new QVBoxLayout(this);


    all->addWidget(logout);
    all->addWidget(activ_users);
    setLayout(all);
    setWindowTitle(tr("Messenger"));
}
void chatwidget::connects(){
    connect(logout,SIGNAL(clicked(bool)),this,SLOT(notactive()));
    connect(list_users,SIGNAL(currentIndexChanged(int)),this,SLOT(connectwithuser()));
    connect(send,SIGNAL(clicked(bool)),this,SLOT(sendmessage()));
}
void chatwidget::connectwithuser(){
   all->addWidget(communicate);
   to_mail=list_users->currentText();
   update_list_messages();
}
void chatwidget::checkonlineusers(){
        QSqlDatabase::database().transaction();
        db.open();
        QSqlQuery query(db);

        if (query.exec("SELECT * FROM my_database.users WHERE active = '1';")) {
            map_active_users.clear();
            while (query.next()) {

                      QString name = query.value(0).toString();
                      QString surname = query.value(1).toString();
                      QString mail = query.value(2).toString();
                      QString phone = query.value(3).toString();
                      //QString image = query.value(5).toString();
                      // QString imagedata = query.value(6).toString();

                      if(mail!=this->mail){
                      map_active_users.insert(mail,onlineuserinfo(name,surname,mail,phone));
                      qDebug() << "Online user: " << mail;
                      qDebug()<<"map_size"<<map_active_users.size();}
            }
            }
         else {
            qDebug() << "Query failed: " << query.lastError().text();
        }

        db.close();
}
void chatwidget::notactive(){

        //openmainwidget();

        QSqlDatabase::database().transaction();
        db.open();
        QSqlQuery query(db);
        bool isActive = 0;

        query.prepare("UPDATE my_database.users SET active = :active WHERE mail = :mail");
        query.bindValue(":active", isActive);
        query.bindValue(":mail", mail); // Provide the user ID for the specific user

        if (query.exec()) {
                  qDebug() << "User status updated successfully";
        } else {
                  qDebug() << "Failed to update user status:" << query.lastError().text();
        }
        db.close();
        QSqlDatabase::database().commit();
        this->close();
}
void chatwidget::showactiveusers(){
        QMapIterator<QString,onlineuserinfo> i(map_active_users);
        list_users->addItem("");
        while(i.hasNext()){
                  i.next();
                  QString labelText = i.key();
                 list_users->addItem(labelText);
        }

}
void chatwidget::setuser(const QString &user){
        mail=user;
}
void chatwidget::sendmessage(){
    if(!message->text().isEmpty() and to_mail != ""){
    QString textuser=QString("%1 : %2 \n").arg(mail).arg(message->text());
    text->insertPlainText(textuser);

    QDateTime currentDateTime = QDateTime::currentDateTime();
    QString datetime = currentDateTime.toString("yyyy-MM-dd hh:mm:ss");

    QSqlDatabase::database().transaction();
    db.open();
    QSqlQuery query(db);
    query.prepare("INSERT INTO my_database.messages (user1,user2,date,message) VALUES (?,?,?,?);");
    query.bindValue(0, mail);
    query.bindValue(1, to_mail);
    query.bindValue(2, datetime);
    query.bindValue(3, message->text());
    //query.bindValue(4, message_id);
    query.exec();
    db.close();

    QSqlDatabase::database().commit();
            message->clear();

    }
}
void chatwidget::update_list_messages(){
    text->clear();
    QSqlDatabase::database().transaction();
    db.open();
    QSqlQuery query;
    query.prepare("SELECT * FROM my_database.messages WHERE user1=:mail and user2=:to_mail");
    query.bindValue(":mail",mail);
    query.bindValue(":to_mail",to_mail);
    if (query.exec()) {
            while (query.next()) {
                      QString message = query.value("message").toString();
                      qDebug() << "Message: " << message;
            }
    } else {
            qDebug() << "Error executing query: " << query.lastError().text();
    }
    db.close();
    QSqlDatabase::database().commit();

}
