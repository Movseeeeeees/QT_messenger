#include "registration.h"
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
   login_page= new QPushButton(tr("Back to login page"));

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
   Lay->addWidget(login_page,10,0);
   Lay->addWidget(password,6,0);
   Lay->addWidget(password_check,7,0);
   setLayout(Lay);
   setWindowTitle(tr("Create account "));

   connect(login_page,SIGNAL(clicked(bool)),this,SLOT(openmainwidget()));
   connect(create,SIGNAL(clicked(bool)) , this,SLOT(create_account()));
   connect(load,SIGNAL(clicked(bool)), this , SLOT(load_image()));
   connect(login_page,SIGNAL(clicked(bool)),this,SLOT(openmainwidget()));
   }
void SecondWidget::insert_data(QString name, QString surname, QString mail, QString phone , QString password,QString imagename , QByteArray fdtos){





   bool activ=false;
    QSqlDatabase::database().transaction();
    db.open();
    QSqlQuery query(db);
    query.prepare("INSERT INTO my_database.users (name,surname,mail,phone,passwordh,imagename,imagedata,active) VALUES (?,?,?,?,?,?,?,?);");
    query.bindValue(0, name);
    query.bindValue(1, surname);
    query.bindValue(2, mail);
    query.bindValue(3, phone);
    query.bindValue(4, password);
    query.bindValue(5, imagename);
    query.bindValue(6, fdtos);
    query.bindValue(7, activ);
    //query.exec();

    if (!query.exec()) {
        qDebug() << "Created"<<db.lastError().text();
        db.rollback();
       }

       // Check if the query was successful
       if (query.numRowsAffected() > 0) {
            qDebug() << "User created successfully";
       } else {
            qDebug() << "Failed to create user";
       }

    db.close();

    QSqlDatabase::database().commit();

      }
void SecondWidget::create_account(){

   QString name_ = name->text();
   QString surname_ = surname->text();
   QString mail_ = mail->text();
   QString phone_ = phone->text();
   QString password_ = hashing(password ->text());
   //check_data();
     if(check_data()){
            insert_data(name_,surname_,mail_,phone_,password_,imagename,fdtos);
           QMessageBox::information(this, "Info", "Succesfuly");
        close();
        }

}
bool SecondWidget::check_data(){

        if(name->text().size()<=2){
            QMessageBox::information(this, "info", "The length of the name must be more than 2 letters");
            return false;
        }
        if(surname->text().size()<=2){
            QMessageBox::information(this, "info", "The length of the surname must be more than 2 letters");
            return false;
       }
        if(mail->text().size()<=8){
            QMessageBox::information(this, "info", "The length of the mail must be more than 8 letters");
            return false;
       }
        if(phone->text().size()<=8){
            QMessageBox::information(this, "info", "The length of the phone must be more than 8 letters");
            return false;
       }
        if(password->text().size()<8){
            QMessageBox::information(this, "info", "The length of the password must be more than 7 letters");
            return false;
       }
        if(password ->text()!= password_check->text()){
            QMessageBox::information(this, "info", "Passwords are not same");
            return false;
       }

      QSqlDatabase::database().transaction();
       db.open();

       QString email=mail->text();
       QString queryStr = "SELECT * FROM my_database.users WHERE mail = :email";

       QSqlQuery query(db);
       query.prepare(queryStr);
       query.bindValue(":email", email);
       query.exec();

       if (query.next()) {
            qDebug() << "Email exists in the database";
            QMessageBox::information(this, "info", "Wrong email");
            return false;
       } else {
            qDebug() << "Email does not exist in the database";
       }

       db.close();

            return true;
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
void SecondWidget::openmainwidget(){
        this->hide();

}
SecondWidget::~SecondWidget(){

}
