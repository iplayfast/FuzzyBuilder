#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qgraphicssceneevent.h>
#include <QFileDialog>

#include "nodefactory.h"
#include "groupitem.h"
namespace Ui {
class MainWindow;

}
class Node;
class MainWindow : public QMainWindow
{
    Q_OBJECT
QString Filename;
public:
    explicit MainWindow(QWidget *parent = 0);
    void SelectNode(Node *np);
    ~MainWindow();

    QString SuggestName(LOGICTYPE t) const;
    bool getFrozen() const;
    void setFrozen(bool value);
    Node *findNode(const QString &Name);
    void Load(QString filename);
    void UpdateTableAndGraph(FuzzyNode *fActive);
    GroupTableItem *getGroupTableItem() const;
    void setGti(GroupTableItem *value);

    void WriteSource(QTextStream &h, QTextStream &s);
    void SaveFile(QString filename);
    void FillCodeUI(Node *np);
private slots:
    void on_actionLoad_triggered();

    void on_actionSave_triggered();

    void on_actionSave_As_C_Source_triggered();

    void on_actionExit_triggered();

    void on_AddLogic_clicked();

    void on_UpdateName_clicked();

    void on_ValueTable_clicked(const QModelIndex &index);

    void on_Min_valueChanged(int value);

    void on_Max_valueChanged(int value);

    void on_pid_valueChanged(int value);

    void on_SetPoint_clicked();



    void on_Lock_clicked();



    void on_ValueTable_entered(const QModelIndex &);
    void Simulate();


    void on_MinScale_valueChanged(int value);

    void on_MaxScale_valueChanged(int value);

    void on_actionView_as_C_Source_triggered();

    void on_DeleteBlock_clicked();

    void on_actionNew_triggered();

    void on_Group_clicked();

    void on_LoadTest_clicked();

    void on_GroupSelect_currentIndexChanged(const QString &arg1);

    void on_DelLogic_clicked();

    void on_EditLogic_clicked();

    void on_actionView_as_Arduino_Source_triggered();

    void on_actionSave_as_Arduiono_Source_triggered();

    void on_FunctionHeader_editingFinished();

    void on_regenerate_clicked();

    void on_tabWidget_currentChanged(int index);

    void on_MinText_editingFinished();

private:
    bool Frozen;
    Ui::MainWindow *ui;
    Node *Active;
    GroupTableItem *gti;
signals:
    void editCompleted(const QString &);
};


extern MainWindow *wp;
#endif // MAINWINDOW_H
