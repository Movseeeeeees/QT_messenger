#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include <QLabel>
#include <QPixmap>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QSqlDatabase>
#include <QGridLayout>
#include <QGroupBox>
#include <QTextEdit>
#include <QMap>
#include "registration.h"
#include <QEvent>
#include <QKeyEvent>

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = 0); //Constructor
    ~MainWidget(); // Destructor
private:
    //main
   QWidget* main;
   QPushButton* button_login;
   QPushButton* button_new;
   QLineEdit* line_log;
   QLineEdit* line_pass;
   QLabel* label_png_m;
   QLabel* label_log;
   QLabel* label_pass;
   //QLabel* label_forgot;
   QPixmap* pixmap_m;
   bool secondWidgetOpen;
   QSqlDatabase db;
   SecondWidget *secondWidget;
protected:
public slots:
    void login();
  // void closed();
   void active(const QString &mail);
   void notactive();
   void openSecondWidget();
   void openchatwidget();
   void openmainwidget();

  //void checkonlineusers();
   QString hashing(const QString &password);
};

#endif // LOGIN_H
