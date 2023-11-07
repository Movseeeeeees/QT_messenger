#include <QWidget>
#include "chatwidget.h"
#include <QGridLayout>
#include <QHBoxLayout>
#include <QMenuBar>
#include <QMenu>
#include <QMainWindow>
#include <QAction>

chatwidget::chatwidget(QWidget *parent):QWidget(parent){

    activ_users= new QGroupBox;
    communicate= new QGroupBox(tr("Area"));

    text = new QTextEdit;
    message = new QLineEdit;
    message->setPlaceholderText("Message");

    send= new QPushButton(tr("Send"));
    logout = new QPushButton(tr("Log out"));

    QWidget *w=new QWidget;
    QHBoxLayout *hbox=new QHBoxLayout;
    hbox->addWidget(message);
    hbox->addWidget(send);
    w->setLayout(hbox);
    QGridLayout *all=new QGridLayout;
    QGridLayout *message_layout= new QGridLayout;
    //all->addWidget(logout,0,0);
    all->addWidget(activ_users,2,0);

    message_layout->addWidget(text,0,0);
    message_layout->addWidget(w,1,0);

    communicate->setLayout(message_layout);
    all->addWidget(communicate,3,0);

    //bar

    QMenuBar *menubar=new QMenuBar(this);

    QMenu *settings= new QMenu("Settings",this);
    QAction *logout_action = new QAction("Log out",this);

    settings->addAction(logout_action);
    menubar->addMenu(settings);

   // setMenuBar(menubar);

    setLayout(all);
    setWindowTitle(tr("Messenger"));
};

