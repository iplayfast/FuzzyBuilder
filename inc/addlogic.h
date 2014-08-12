#ifndef ADDLOGIC_H
#define ADDLOGIC_H

#include <QDialog>
#include "TFuzzy.h"
#include "mainwindow.h"
#include "nodefactory.h"

namespace Ui {
class AddLogic;
}

class AddLogic : public QDialog
{
    Q_OBJECT
MainWindow *wp;
public:
    explicit AddLogic(MainWindow *mwp,QWidget *parent = 0);
    ~AddLogic();
    QString GetName() const;
    LOGICTYPE GetLogicType() const;
    int GetMax() const;
    int GetMin() const;
    QString getINLINE() const;
    void Load(Node *v);
    void HideType();
private slots:
    void on_buttonBox_accepted();

    void SelectType(LOGICTYPE ltv);

    void on_In_clicked();

    void on_Out_clicked();

    void on_And_clicked();

    void on_Or_clicked();

    void on_Not_clicked();

    void on_Fuzzy_clicked();

    void on_Pid_clicked();

    void on_Timer_clicked();

    void on_Setup_clicked();

private:


    Ui::AddLogic *ui;
    LOGICTYPE lt;
};

#endif // ADDLOGIC_H
