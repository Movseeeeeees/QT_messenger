#ifndef MAINWIDGET_H
#define MAINWIDGET_H
#include <QWidget>
#include <QLabel>
#include <QPixmap>
#include <QHBoxLayout>

class QPushButton;
class QLineEdit;

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = 0); //Constructor
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

};

#endif // MAINWIDGET_H
