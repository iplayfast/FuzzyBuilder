#ifndef ADDLOGIC_H
#define ADDLOGIC_H

#include <QDialog>
#include "TFuzzy.h"
#include "mainwindow.h"


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
private slots:
    void on_lineEdit_editingFinished();

    void on_comboBox_currentIndexChanged(const QString &arg1);

    void on_buttonBox_accepted();

    void on_Min_sliderMoved(int position);

    void on_Max_sliderMoved(int position);

    void on_dial_sliderMoved(int position);

    void on_Max_valueChanged(int value);

    void on_Min_valueChanged(int value);

private:
    QString GetInputGuts() const;
    QString GetOutputGuts() const;
    Ui::AddLogic *ui;
    LOGICTYPE lt;
};

#endif // ADDLOGIC_H
