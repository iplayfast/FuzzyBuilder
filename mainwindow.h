#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qgraphicssceneevent.h>
#include "nodefactory.h"
namespace Ui {
class MainWindow;

}
class Node;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void DisplayNode(Node *np);
    ~MainWindow();

    QString SuggestName(LOGICTYPE t) const;
    bool getFrozen() const;
    void setFrozen(bool value);

    void Load(QString filename);
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

    void on_NodeName_editingFinished();

//    void on_NodeName_textChanged(const QString &arg1);

//    void on_NodeName_textEdited(const QString &arg1);

    void on_NodeName_returnPressed();

    void on_graphicsView_rubberBandChanged(const QRect &viewportRect, const QPointF &fromScenePoint, const QPointF &toScenePoint);

private:
    bool Frozen;
    Ui::MainWindow *ui;
    Node *Active;
    void setMin(bool Vis,const char *text,double v);
    void setMax(bool Vis,const char *text,double v);
    void setPID(bool Vis,const char *text,double v);
};


extern MainWindow *wp;
#endif // MAINWINDOW_H
