#include "viewsourcedialog.h"
#include "ui_viewsourcedialog.h"

ViewSourceDialog::ViewSourceDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ViewSourceDialog)
{
    ui->setupUi(this);
}

ViewSourceDialog::~ViewSourceDialog()
{
    delete ui;
}

void ViewSourceDialog::SetSource(QString h, QString s)
{
    ui->label->setVisible(true);
    ui->Header->setVisible(true);
    ui->Header->setText(h);
    ui->Source->setText(s);
}

void ViewSourceDialog::SetSource(QString s)
{
    ui->label->setVisible(false);
    ui->Header->setVisible(false);
    ui->Source->setText(s);
}
