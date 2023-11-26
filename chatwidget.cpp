#include <QWidget>
#include "chatwidget.h"
#include "login.h"
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
#include <QTimer>
#include <QUrl>
#include <QBitmap>
#include <QPainter>
#include <QListWidget>
#include <QListWidgetItem>

chatwidget::chatwidget(const QString &user, QWidget *parent):QWidget(parent){

    setuser(user);

    database_init();

    design();

    get_users_from_db();

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

    //first hbox
    QWidget *status = new QWidget;
    logout = new QPushButton(tr("Log out"));
    username = new QLabel(mail);
    QHBoxLayout *hbox_up = new QHBoxLayout;
    hbox_up->addWidget(get_image_from_db(mail));
    hbox_up->addWidget(username);
    hbox_up->addWidget(logout);
    status->setLayout(hbox_up);
    // end

    //active users groupbox
    activ_users= new QGroupBox(tr("Active users"));
    activ_users->setFixedHeight(100);
    group_lay_active = new QHBoxLayout;
    activ_users->setLayout(group_lay_active);
    active_users_list = new QListWidget;
    group_lay_active->addWidget(active_users_list);
    //end

    //communicate group box
    communicate= new QGroupBox(tr("Area"));
    message = new QLineEdit;
    message->setPlaceholderText("Message");
    send= new QPushButton(tr("Send"));
    QWidget *w=new QWidget;
    QHBoxLayout *hbox=new QHBoxLayout;
    hbox->addWidget(message);
    hbox->addWidget(send);
    w->setLayout(hbox);
    text = new QTextBrowser;
    group_lay_communicate= new QVBoxLayout;
    group_lay_communicate->addWidget(text);
    group_lay_communicate->addWidget(w);
    communicate->setLayout(group_lay_communicate);
    // end

    //list of users
    QWidget *user_list_w=new QWidget;
    l=new QHBoxLayout;
    users_list=new QListWidget;
    l->addWidget(users_list);
    user_list_w->setLayout(l);
    //end


    //set design
    all=new QVBoxLayout(this);
    all->addWidget(status);
    all->addWidget(activ_users);
    all->addWidget(user_list_w);
    setLayout(all);
    setWindowTitle(tr("Messenger"));
}
void chatwidget::connects(){
    connect(logout,SIGNAL(clicked(bool)),this,SLOT(notactive()));
    connect(send,SIGNAL(clicked(bool)),this,SLOT(sendmessage()));
    connect(users_list,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(connectwithuser()));
    connect(active_users_list,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(connectwithuser_from_online()));

    QTimer *timer = new QTimer(this);

    connect(timer,SIGNAL(timeout()),this,SLOT(update_list_messages()));
    connect(timer,SIGNAL(timeout()),this,SLOT(get_users_from_db()));
    connect(timer,SIGNAL(timeout()),this,SLOT(checkonlineusers()));
    connect(timer,SIGNAL(timeout()),this,SLOT(check_new_message()));

    timer->start(2000);

}
void chatwidget::connectwithuser(){
    l->addWidget(communicate);
    to_mail=users_list->currentItem()->text();
    update_list_messages();
    users_list->currentItem()->setBackground(Qt::color0);
    set_messages_statuse_read(to_mail,mail);
}
void chatwidget::connectwithuser_from_online(){
    l->addWidget(communicate);
    to_mail=active_users_list->currentItem()->text();
    update_list_messages();
    set_messages_statuse_read(to_mail,mail);
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
                      if(mail!=this->mail){
                      map_active_users.insert(mail,onlineuserinfo(name,surname,mail,phone));
                      }
            }
            }
         else {
            qDebug() << "Query failed: " << query.lastError().text();
        }

        db.close();
        showactiveusers();
}
void chatwidget::notactive(){
        QSqlDatabase::database().transaction();
        db.open();
        QSqlQuery query(db);
        bool isActive = 0;
        query.prepare("UPDATE my_database.users SET active = :active WHERE mail = :mail");
        query.bindValue(":active", isActive);
        query.bindValue(":mail", mail);
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
        active_users_list->clear();
        while(i.hasNext()){
                  i.next();
                  QString labelText = i.key();
                  active_users_list->addItem(labelText);
        }
}
void chatwidget::setuser(const QString &user){
        mail=user;
}
void chatwidget::sendmessage(){
    if(!message->text().isEmpty() and to_mail != ""){
    QString textuser=QString("%1 : %2 \n").arg(mail).arg(message->text());
    text->insertPlainText(textuser);
    text->moveCursor(QTextCursor::End);
    QDateTime currentDateTime = QDateTime::currentDateTime();
    QString datetime = currentDateTime.toString("yyyy-MM-dd hh:mm:ss");
    QString status="new";

    QSqlDatabase::database().transaction();
    db.open();
    QSqlQuery query(db);
    query.prepare("INSERT INTO my_database.messages (user1,user2,date,message,status) VALUES (?,?,?,?,?);");
    query.bindValue(0, mail);
    query.bindValue(1, to_mail);
    query.bindValue(2, datetime);
    query.bindValue(3, message->text());
    query.bindValue(4, status);
    query.exec();
    db.close();
    QSqlDatabase::database().commit();
    message->clear();
    }
}
void chatwidget::update_list_messages(){
   if(to_mail != ""){
    text->clear();
    QSqlDatabase::database().transaction();
    db.open();
    QSqlQuery query;
    query.prepare("SELECT * FROM my_database.messages WHERE (user1=:mail or user1=:to_mail) and (user2=:to_mail or user2=:mail)");
    query.bindValue(":mail",mail);
    query.bindValue(":to_mail",to_mail);
    if (query.exec()) {
            while (query.next()) {
                      QString user = query.value("user1").toString();
                      QString message = query.value("message").toString();
                      QString textuser = QString("%1 : %2 \n").arg(user).arg(message);
                      text->insertPlainText(textuser);
            }
            text->moveCursor(QTextCursor::End);
    } else {
            qDebug() << "Error executing query: " << query.lastError().text();
    }
    db.close();
    QSqlDatabase::database().commit();
   }}
QLabel* chatwidget::get_image_from_db(QString user){
        QByteArray image_data;
        QPixmap *image = new QPixmap("/home/movses/QT_messenger/user.png");
        QString image_name;

        QSqlDatabase::database().transaction();
        db.open();
        QSqlQuery query(db);
        query.prepare("SELECT * FROM my_database.users WHERE mail=:user");
        query.bindValue(":user",user);
        if(query.exec()){
            if(query.next()){
            image_name=query.value("imagename").toString();
            image_data=QByteArray::fromBase64(query.value("imagedata").toByteArray());
            }
        }
        else{
            qDebug() << "Error executing query: " << query.lastError().text();
            return nullptr;
        }
        db.close();
        QSqlDatabase::database().commit();


        if(!image_data.isEmpty()){

        image->loadFromData(image_data,"JPG");

        }
        *image = image->scaled(100,100,Qt::KeepAspectRatio);

        QLabel *label_png = new QLabel;
        label_png->setPixmap(*image);
        QBitmap mask(image->size());
        mask.fill(Qt::color0);
        QPainter painter(&mask);
        painter.setBrush(Qt::color1);
        painter.drawEllipse(0,0,image->width(),image->width());
        label_png->setMask(mask);
        label_png->setFixedSize(100,100);
        return label_png;
}
void chatwidget::get_users_from_db(){
        QSqlDatabase::database().transaction();
        db.open();
        QSqlQuery query(db);
        map_all_users.clear();
        users_list->clear();
        if (query.exec("SELECT * FROM my_database.users")) {
            while (query.next()) {

                      QString name = query.value(0).toString();
                      QString surname = query.value(1).toString();
                      QString mail = query.value(2).toString();
                      QString phone = query.value(3).toString();
                      //bool active = query.value(7).toBool();

                      if(mail!=this->mail){
                           map_all_users.insert(mail,onlineuserinfo(name,surname,mail,phone));
                      }
                      }
                       QList<QString> keys = map_all_users.keys();
                       for (int i = 0; i < keys.size(); ++i) {
                       //qDebug() << "Index:" << i << "Key:" << keys.at(i);
                       QListWidgetItem *item=new QListWidgetItem(keys.at(i));
                       users_list->addItem(item);
                       }

        }

         else {
            qDebug() << "Query failed: " << query.lastError().text();
        }

        db.close();
}
void chatwidget::check_new_message(){
        QSqlDatabase::database().transaction();
        db.open();
        QSqlQuery query(db);
        QString user="new";
        query.exec("SELECT * FROM my_database.messages WHERE (user2=?) AND (status=?)");
        query.bindValue(0,mail);
        query.bindValue(1,user);

        if(query.exec()){
            while (query.next()) {
                      QString from = query.value(0).toString();
                      //QString messag= query.value(3).toString();
                      //qDebug() << "message from " << from <<"  "<<messag;
                      //show from who
                      QList<QString> keys = map_all_users.keys();
                      int index = map_all_users.keys().indexOf(from);
                      //qDebug() <<map_all_users.size()<< "message from " << index;
                      users_list->item(index)->setBackground(Qt::yellow);
            }
            }
         else {
            qDebug() << "Query failed: " << query.lastError().text();
        }
        db.close();
}
void chatwidget::set_messages_statuse_read(QString user1, QString user2){
        QSqlDatabase::database().transaction();
        db.open();
        QSqlQuery query(db);
        QString status="Readed";
        query.prepare("UPDATE my_database.messages SET status = :status WHERE user1 = :user1 and user2=:user2");
        query.bindValue(":status", status);
        query.bindValue(":user1",user1);
        query.bindValue(":user2", user2);
        if (query.exec()) {
                  qDebug() << "Messages status updated successfully";
        } else {
                  qDebug() << "Failed to update messages status:" << query.lastError().text();
        }
        db.close();
        QSqlDatabase::database().commit();
}
