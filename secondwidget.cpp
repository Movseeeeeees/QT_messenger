#include "secondwidget.h"
#include <QWidget>
#include <QLabel>
#include <QMessageBox>
#include <QString>
#include <QSqlQuery>
#include <QSqlError>
#include <QtSql/QSql>
#include <QSqlDatabase>
#include <QCryptographicHash>
#include <QFileDialog>
#include <QPixmap>
#include <QBuffer>
#include <QFileInfo>

SecondWidget::SecondWidget(QWidget *parent) : QWidget(parent)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("127.0.0.1");
    db.setDatabaseName("my_database");
    db.setUserName("hp");
    db.setPassword("7777777");
    db.setPort(3306);

   name=new QLineEdit;
   surname=new QLineEdit;
   mail=new QLineEdit;
   phone=new QLineEdit;
   password=new QLineEdit;
   password_check=new QLineEdit;
   create=new QPushButton(tr("Create new account"));
   load = new QPushButton(tr("Load image"));
   name->setPlaceholderText("Name");
   surname->setPlaceholderText("Surname");
   mail->setPlaceholderText("Mail");
   phone->setPlaceholderText("Phone");
   password->setPlaceholderText("Password");
   password_check->setPlaceholderText("Password again");

   password->setEchoMode(QLineEdit::Password);
   password_check->setEchoMode(QLineEdit::Password);
   QGridLayout *Lay = new QGridLayout;

   pixmap = new QPixmap("/home/movses/QT_messenger/registration.png");
   label_png = new QLabel;
   label_png->setPixmap(*pixmap);

   Lay->addWidget(label_png,1,0);
   Lay->addWidget(name,2,0);
   Lay->addWidget(surname,3,0);
   Lay->addWidget(mail,4,0);
   Lay->addWidget(phone,5,0);
   Lay->addWidget(create,9 ,0);
   Lay->addWidget(load,8,0);
   Lay->addWidget(password,6,0);
   Lay->addWidget(password_check,7,0);
   setLayout(Lay);
   setWindowTitle(tr("Create account "));

   connect(create,SIGNAL(clicked(bool)) , this,SLOT(create_account()));
   connect(load,SIGNAL(clicked(bool)), this , SLOT(load_image()));
   }
void SecondWidget::insert_data(QString name, QString surname, QString mail, QString phone , QString password,QString imagename , QByteArray fdtos){

    QSqlDatabase::database().transaction();
    db.open();
    QSqlQuery query(db);
    query.prepare("INSERT INTO my_database.users (name,surname,mail,phone,passwordh,imagename,imagedata) VALUES (?,?,?,?,?,?,?);");
    query.bindValue(0, name);
    query.bindValue(1, surname);
    query.bindValue(2, mail);
    query.bindValue(3, phone);
    query.bindValue(4, password);
    query.bindValue(5, imagename);
    query.bindValue(6, fdtos);

    query.exec();
    db.close();

    QSqlDatabase::database().commit();

      }
void SecondWidget::create_account(){

   QString name_ = name->text();
   QString surname_ = surname->text();
   QString mail_ = mail->text();
   QString phone_ = phone->text();
   QString password_ = hashing(password ->text());

     if(password ->text()== password_check->text()){
            insert_data(name_,surname_,mail_,phone_,password_,imagename,fdtos);
           QMessageBox::information(this, "Info", "Succesfuly");
        close();
        }
        else {
            QMessageBox::information(this, "Info", "Not same passwords");
        }
}
QString SecondWidget::hashing(const QString &password){

        QByteArray passwordBytes = password.toUtf8();
        QByteArray hashBytes = QCryptographicHash::hash(passwordBytes, QCryptographicHash::Sha256);


        return QString(hashBytes.toHex());

}
void SecondWidget::load_image(){

        QString filename = QFileDialog::getOpenFileName(this,tr("Choose"),"",tr("Images(*.png *.jpg *.jpeg *.bmp *.gif)"));
        QPixmap image(filename);
        QBuffer bufferdata;
        if(bufferdata.open(QIODevice::ReadWrite)){
            image.save(&bufferdata,"JPG");
        }

        fdtos = bufferdata.buffer().toBase64();

        QFileInfo fileinfo(filename);
        imagename = fileinfo.fileName();
}
SecondWidget::~SecondWidget(){


}
