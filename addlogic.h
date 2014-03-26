#ifndef ADDLOGIC_H
#define ADDLOGIC_H

#include <QDialog>

namespace Ui {
class AddLogic;
}

class AddLogic : public QDialog
{
    Q_OBJECT

public:
    explicit AddLogic(QWidget *parent = 0);
    ~AddLogic();

private:
    Ui::AddLogic *ui;
};

#endif // ADDLOGIC_H
