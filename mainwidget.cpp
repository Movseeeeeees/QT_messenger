#include <QtWidgets>
#include "mainwidget.h"
#include "chatwidget.h"
#include <QtSql/QSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QString>
#include <QLabel>
#include <QMessageBox>
#include <QCryptographicHash>
#include <QFileDialog>
#include <QPixmap>
#include <QBuffer>
#include <QFileInfo>
#include <QCloseEvent>
#include <QMap>

MainWidget::MainWidget(QWidget *parent) :QWidget(parent){


    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("127.0.0.1");
    db.setDatabaseName("my_database");
    db.setUserName("hp");
    db.setPassword("7777777");
    db.setPort(3306);

   main= new QWidget;
 //  main->setWindowIcon(QIcon("/home/movses/QT_messenger/chat.png"));
   button_new = new QPushButton(tr("Create new account"));
   button_login = new QPushButton(tr("Log In"));
   button_login->setDefault(true);
   line_log = new QLineEdit;
   line_pass = new QLineEdit;
   label_log = new QLabel("Login");
   label_pass = new QLabel("Password");
   line_pass->setEchoMode(QLineEdit::Password);
   //label_forgot = new QLabel("Forgot password?");
   pixmap_m = new QPixmap("/home/movses/QT_messenger/chat.png");
   label_png_m = new QLabel;

   QGridLayout *mainLayout = new QGridLayout;
   label_png_m->setPixmap(*pixmap_m);
   label_png_m->setFixedSize(250,250);

   mainLayout->addWidget(label_png_m,1,0);
   mainLayout->addWidget(label_log,2,0);
   mainLayout->addWidget(line_log,3,0);
   mainLayout->addWidget(label_pass,4,0);
   mainLayout->addWidget(line_pass,5,0);
   //mainLayout->addWidget(label_forgot,6,0);

   QWidget *widget = new QWidget;
   QHBoxLayout *horizontalLayout = new QHBoxLayout;

   horizontalLayout->addWidget(button_new);
   horizontalLayout->addWidget(button_login);

   widget->setLayout(horizontalLayout);
   mainLayout->addWidget(widget,6,0);


   main->setLayout(mainLayout);
   main->setWindowTitle(tr("Messenger"));
   connect(button_new, &QPushButton::clicked, this,&MainWidget::openSecondWidget);
   connect(button_login,SIGNAL(clicked(bool)),this,SLOT(login()));
   main->show();

}
void MainWidget::login(){
        if(line_log->text().isEmpty() or line_pass->text().isEmpty()){
                  QMessageBox::information(this, "Info", "wrong");
        }
        else{

                  QString username = line_log->text();
                  QString password = hashing(line_pass->text());

                  QSqlDatabase::database().transaction();
                  db.open();
                  QSqlQuery query(db);

                  query.prepare("SELECT * FROM my_database.users WHERE mail = :username AND passwordh = :password");
                  query.bindValue(":username", username);
                  query.bindValue(":password", password);
                  query.exec();

                  if(query.next()) {
                      qDebug() << "Login successful";
                      //QMessageBox::information(this, "Info", "Succesfuly");

                      active(username);
                      openchatwidget();
                      main->hide();
                      this->setVisible(false);

                  } else {
                      QMessageBox::information(this, "Info", "Login failed. Incorrect username or password.");
                      qDebug() << "Login failed. Incorrect username or password.";
                      qDebug() << "Error details: " << query.lastError();
                  }
                 db.close();
                  QSqlDatabase::database().commit();
            }
}
QString MainWidget::hashing(const QString &password){

        QByteArray passwordBytes = password.toUtf8();
        QByteArray hashBytes = QCryptographicHash::hash(passwordBytes, QCryptographicHash::Sha256);


        return QString(hashBytes.toHex());

}
void MainWidget::active(const QString &mail){


        QSqlDatabase::database().transaction();
        db.open();
        QSqlQuery query(db);
        bool isActive = true;

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

}
void MainWidget::notactive(){

        //openmainwidget();

        QSqlDatabase::database().transaction();
        db.open();
        QSqlQuery query(db);
        bool isActive = false;

        query.prepare("UPDATE my_database.users SET active = :active WHERE mail = :mail");
        query.bindValue(":active", isActive);
        query.bindValue(":mail", line_log->text()); // Provide the user ID for the specific user

        if (query.exec()) {
                  qDebug() << "User status updated successfully";
        } else {
                  qDebug() << "Failed to update user status:" << query.lastError().text();
        }
        db.close();
        QSqlDatabase::database().commit();
}
void MainWidget::openSecondWidget(){
       // if (!secondWidgetOpen)
       // {
                  secondWidget = new SecondWidget;
                  this->hide();
                  secondWidget->show();
                  //secondWidgetOpen=true;
       // }
        //else
       /* {

                  QMessageBox::information(this, "Info", "Second window is already open.");
        }*/
}
void MainWidget::openchatwidget(){
                  chatwidget *chat=new chatwidget(line_log->text(),nullptr);
        chat->resize(400,400);
        chat->show();
}
void MainWidget::openmainwidget(){
       secondWidget->hide();

        this->show();
}
MainWidget::~MainWidget()
{
   delete button_new;
   delete button_login;
   delete line_log;
   delete line_pass; 
   delete label_log;
   delete label_pass;
   //delete label_forgot;
   //delete label_png_c;
   //delete pixmap_c;
}

