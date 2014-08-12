#ifndef GROUPS_H
#define GROUPS_H

#include <QDialog>
#include "groupitem.h"
#include "node.h"
#include "mainwindow.h"
namespace Ui {
class Groups;
}

class Groups : public QDialog
{
    Q_OBJECT
Node *CurrentNode;
public:
    explicit Groups(QWidget *parent = 0);
    ~Groups();

    void addNode(const QString &NodeName);




    void updateTable();
private slots:
    void on_AddGroup_clicked();

    void on_NodeList_activated(const QString &NodeName);

    void on_NodeList_currentIndexChanged(const QString &NodeName);

private:
    Ui::Groups *ui;
};

#endif // GROUPS_H
