#include <QtWidgets>
#include "mainwidget.h"

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent)
{
   button_new = new QPushButton(tr("Create new account"));
   button_login = new QPushButton(tr("Log In"));
   line_log = new QLineEdit;
   line_pass = new QLineEdit;
   label_log = new QLabel("Login");
   label_pass = new QLabel("Password");
   line_pass->setEchoMode(QLineEdit::Password);
   label_forgot = new QLabel("Forgot password?");
   pixmap = new QPixmap("chat.png");
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
