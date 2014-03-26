#include "addlogic.h"
#include "ui_addlogic.h"

AddLogic::AddLogic(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddLogic)
{
    ui->setupUi(this);
}

AddLogic::~AddLogic()
{
    delete ui;
}
