#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPixmap>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QSqlDatabase>
class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = 0); //Constructor
    ~MainWidget(); // Destructor

private:
   QPushButton* button_login;
   QPushButton* button_new;
   QLineEdit* line_log;
   QLineEdit* line_pass;
   QLabel* label_png;
   QLabel* label_log;
   QLabel* label_pass;
   QLabel* label_forgot;
   QPixmap* pixmap;
   bool secondWidgetOpen;
   QSqlDatabase db;
public slots:
   void openSecondWidget();
   void openchatwidget();
   void login();
   QString hashing(const QString &password);
   void closed();
};

#endif // MAINWIDGET_H
