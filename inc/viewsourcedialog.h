#ifndef VIEWSOURCEDIALOG_H
#define VIEWSOURCEDIALOG_H

#include <QDialog>

namespace Ui {
class ViewSourceDialog;
}

class ViewSourceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ViewSourceDialog(QWidget *parent = 0);
    ~ViewSourceDialog();
   void SetSource(QString h,QString s);
   void SetSource(QString s);
private:
    Ui::ViewSourceDialog *ui;
};

#endif // VIEWSOURCEDIALOG_H
