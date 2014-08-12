#include "groups.h"
#include "ui_groups.h"
#include "mainwindow.h"
Groups::Groups(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Groups)
{
    ui->setupUi(this);    

    ui->TableValue->setModel(wp->getGroupTableItem());
    ui->TableValue->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Stretch);
    ui->TableValue->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);

    CurrentNode = 0;
}

Groups::~Groups()
{    
    delete ui;
}

void Groups::addNode(const QString &NodeName)
{
    ui->NodeList->addItem(NodeName);
}


void Groups::updateTable()
{
    ui->TableValue->setModel(0);
    ui->TableValue->setModel(wp->getGroupTableItem());
    ui->TableValue->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Stretch);
    ui->TableValue->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
    ui->TableValue->update();
}

void Groups::on_AddGroup_clicked()
{
    wp->getGroupTableItem()->AddGroupItem(wp->getGroupTableItem()->rowCount(),"blank");
    updateTable();
}

void Groups::on_NodeList_activated(const QString &NodeName)
{
    on_NodeList_currentIndexChanged(NodeName);
}


void Groups::on_NodeList_currentIndexChanged(const QString &NodeName)
{
    CurrentNode =  wp->findNode(NodeName);
    wp->getGroupTableItem()->ActiveNode = CurrentNode;
    updateTable();
}
