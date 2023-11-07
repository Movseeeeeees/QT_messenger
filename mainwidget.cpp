#include <QtWidgets>
#include "mainwidget.h"
#include "secondwidget.h"
#include "chatwidget.h"
#include <QtSql/QSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QString>

MainWidget::MainWidget(QWidget *parent) :QWidget(parent){
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("127.0.0.1");
    db.setDatabaseName("my_database");
    db.setUserName("hp");
    db.setPassword("7777777");
    db.setPort(3306);

   button_new = new QPushButton(tr("Create new account"));
   button_login = new QPushButton(tr("Log In"));
   line_log = new QLineEdit;
   line_pass = new QLineEdit;
   label_log = new QLabel("Login");
   label_pass = new QLabel("Password");
   line_pass->setEchoMode(QLineEdit::Password);
   label_forgot = new QLabel("Forgot password?");
   pixmap = new QPixmap("/home/movses/QT_messenger/chat.png");
   label_png = new QLabel;

   QGridLayout *mainLayout = new QGridLayout;
   label_png->setPixmap(*pixmap);
   label_png->setFixedSize(250,250);

   mainLayout->addWidget(label_png,1,0);   
   mainLayout->addWidget(label_log,2,0);
   mainLayout->addWidget(line_log,3,0);
   mainLayout->addWidget(label_pass,4,0);
   mainLayout->addWidget(line_pass,5,0);
   mainLayout->addWidget(label_forgot,6,0);

   QWidget *widget = new QWidget;
   QHBoxLayout *horizontalLayout = new QHBoxLayout;

   horizontalLayout->addWidget(button_new);
   horizontalLayout->addWidget(button_login);

   widget->setLayout(horizontalLayout);
   mainLayout->addWidget(widget,7,0);


   setLayout(mainLayout);
   setWindowTitle(tr("Messenger"));
   connect(button_new, &QPushButton::clicked, this,&MainWidget::openSecondWidget);
   connect(button_login,SIGNAL(clicked(bool)),this,SLOT(login()));
}
void MainWidget::closed(){
   secondWidgetOpen=false;
}
void MainWidget::openSecondWidget(){
   if (!secondWidgetOpen)
   {
       SecondWidget *secondWidget = new SecondWidget;
       secondWidget->show();
       secondWidgetOpen=true;
       }
   else
   {

               QMessageBox::information(this, "Info", "Second window is already open.");
   }
    }
void MainWidget::openchatwidget(){
   chatwidget *chat=new chatwidget;
   chat->resize(400,400);
   chat->show();
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
                      QMessageBox::information(this, "Info", "Succesfuly");
                      openchatwidget();
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
MainWidget::~MainWidget()
{
   delete button_new;
   delete button_login;
   delete line_log;
   delete line_pass; 
   delete label_log;
   delete label_pass;
   delete label_forgot;          
   delete label_png;
   delete pixmap;
}
