#include "secondwidget.h"
#include <QWidget>
#include <QLabel>
SecondWidget::SecondWidget(QWidget *parent) : QWidget(parent)
{

   name=new QLineEdit;
   surname=new QLineEdit;
   mail=new QLineEdit;
   phone=new QLineEdit;
   create=new QPushButton(tr("Create new account"));
   load = new QPushButton(tr("Load image"));
   name->setPlaceholderText("Name");
   surname->setPlaceholderText("Surname");
   mail->setPlaceholderText("Mail");
   phone->setPlaceholderText("Phone");
   QGridLayout *Lay = new QGridLayout;

   pixmap = new QPixmap("/home/movses/QT_messenger/registration.png");
   label_png = new QLabel;
   label_png->setPixmap(*pixmap);

   Lay->addWidget(label_png,1,0);
   Lay->addWidget(name,2,0);
   Lay->addWidget(surname,3,0);
   Lay->addWidget(mail,4,0);
   Lay->addWidget(phone,5,0);
   Lay->addWidget(create,7 ,0);
   Lay->addWidget(load,6,0);
   setLayout(Lay);
   setWindowTitle(tr("Create account "));

  }
SecondWidget::~SecondWidget(){

}
